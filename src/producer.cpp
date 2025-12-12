#include "include/order.hpp"
#include "include/queue.hpp"
#include "include/config.hpp"
#include <random>
#include <thread>
#include <atomic>
#include <sstream>
#include <iomanip>
#include <iostream>

// simple thread-safe queue usage: instantiate and use (the ThreadSafeQueue<Order> is explicitly instantiated in threadsafe_queue.cpp)
void producer_thread(ThreadSafeQueue<Order> &q, std::atomic<bool> &run_flag, std::atomic<uint64_t> &id_counter, Metrics &metrics) {
    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_real_distribution<double> price_dist(99.0, 101.0);
    std::uniform_int_distribution<int> side_dist(0,1);
    std::uniform_int_distribution<int> qty_dist(1, 100);

    while (run_flag.load()) {
        uint64_t id = id_counter.fetch_add(1);
        Side side = side_dist(gen) == 0 ? Side::BUY : Side::SELL;
        double price = std::round(price_dist(gen) * 100.0) / 100.0; // 2 decimal places
        uint64_t qty = (uint64_t)qty_dist(gen);

        Order o{id, side, price, qty};
        metrics.orders_received.fetch_add(1);

        q.push(o);

        // control generation rate
        std::this_thread::sleep_for(std::chrono::microseconds(cfg::PRODUCER_SLEEP_US));
    }
}
