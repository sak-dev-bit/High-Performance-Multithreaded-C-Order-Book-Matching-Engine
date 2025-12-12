#pragma once
#include <chrono>
#include <atomic>

struct Metrics {
    std::atomic<uint64_t> orders_received{0};
    std::atomic<uint64_t> orders_processed{0};
    std::atomic<uint64_t> matches{0};

    std::atomic<uint64_t> total_latency_ns{0}; // sum of latencies
    std::atomic<uint64_t> max_latency_ns{0};

    void record_latency(uint64_t ns) {
        total_latency_ns += ns;
        uint64_t prev_max = max_latency_ns.load();
        while (ns > prev_max && !max_latency_ns.compare_exchange_weak(prev_max, ns)) {}
    }

    double average_latency_ms() const {
        uint64_t processed = orders_processed.load();
        if (processed == 0) return 0.0;
        return (double)total_latency_ns.load() / processed / 1e6;
    }
};
