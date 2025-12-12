#include "include/order.hpp"
#include "include/queue.hpp"
#include "include/order_book.hpp"
#include "include/metrics.hpp"
#include "include/config.hpp"

#include <thread>
#include <atomic>
#include <iostream>
#include <chrono>

extern void producer_thread(ThreadSafeQueue<Order> &q, std::atomic<bool> &run_flag, std::atomic<uint64_t> &id_counter, Metrics &metrics);
extern void consumer_thread(ThreadSafeQueue<Order> &q, std::atomic<bool> &run_flag, OrderBook &ob, Metrics &metrics);
extern void print_metrics(const Metrics &m);

int main() {
    ThreadSafeQueue<Order> q;
    OrderBook ob;
    Metrics metrics;

    std::atomic<bool> run_flag{true};
    std::atomic<uint64_t> id_counter{1};

    std::thread prod([&]{ producer_thread(q, run_flag, id_counter, metrics); });
    std::thread cons([&]{ consumer_thread(q, run_flag, ob, metrics); });

    auto start = std::chrono::steady_clock::now();
    auto last_report = start;

    // run for configured duration
    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - start).count();
        auto since_report_ms = std::chrono::duration_cast<std::chrono::milliseconds>(now - last_report).count();
        if (since_report_ms >= cfg::METRICS_REPORT_INTERVAL_MS) {
            print_metrics(metrics);
            last_report = now;
        }
        if (elapsed >= cfg::MAX_RUNTIME_SECONDS) break;
    }

    // stop
    run_flag.store(false);
    q.shutdown();
    if (prod.joinable()) prod.join();
    if (cons.joinable()) cons.join();

    // final metrics + orderbook summary
    print_metrics(metrics);
    std::cout << "Total matches recorded by orderbook: " << ob.total_matches_executed() << std::endl;
    std::cout << "Exiting." << std::endl;
    return 0;
}
