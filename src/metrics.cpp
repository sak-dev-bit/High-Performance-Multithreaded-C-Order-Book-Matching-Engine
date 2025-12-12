#include "include/metrics.hpp"
#include <iostream>
#include <iomanip>

void print_metrics(const Metrics &m) {
    uint64_t received = m.orders_received.load();
    uint64_t processed = m.orders_processed.load();
    uint64_t matches = m.matches.load();
    double avg_ms = m.average_latency_ms();
    uint64_t max_ns = m.max_latency_ns.load();
    double max_ms = max_ns / 1e6;

    std::cout << "=== Metrics ===\n";
    std::cout << "Orders received: " << received << "\n";
    std::cout << "Orders processed: " << processed << "\n";
    std::cout << "Matches executed: " << matches << "\n";
    std::cout << "Avg latency (ms): " << std::fixed << std::setprecision(3) << avg_ms << "\n";
    std::cout << "Max latency (ms): " << std::fixed << std::setprecision(3) << max_ms << "\n";
    std::cout << "================\n";
}
