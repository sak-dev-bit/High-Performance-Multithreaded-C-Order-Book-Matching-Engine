#include "include/order.hpp"
#include "include/queue.hpp"
#include "include/order_book.hpp"
#include "include/metrics.hpp"
#include "include/config.hpp"
#include <thread>
#include <atomic>
#include <chrono>
#include <sstream>
#include <iostream>

// consumer reads orders, processes with OrderBook, records metrics, and logs matches
void consumer_thread(ThreadSafeQueue<Order> &q, std::atomic<bool> &run_flag, OrderBook &ob, Metrics &metrics) {
    while (run_flag.load() || !q.empty()) {
        Order o;
        bool ok = q.pop(o);
        if (!ok) continue; // shutdown signalled and queue empty

        auto now = std::chrono::high_resolution_clock::now();
        uint64_t latency_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(now - o.created_ts).count();
        metrics.record_latency(latency_ns);
        metrics.orders_processed.fetch_add(1);

        auto matches = ob.process_order(o);
        if (!matches.empty()) {
            metrics.matches.fetch_add(matches.size());
            // simple logging to console
            for (const auto &m : matches) {
                std::ostringstream ss;
                ss << "Matched: BUY[" << m.buy_order_id << "] SELL[" << m.sell_order_id << "] "
                   << "QTY=" << m.quantity << " PRICE=" << m.price;
                // printing directly (std::cout is fine but keep short)
                std::cout << ss.str() << std::endl;
            }
        }
    }
}
