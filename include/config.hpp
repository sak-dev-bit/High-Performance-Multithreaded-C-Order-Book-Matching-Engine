#pragma once

namespace cfg {
    constexpr int PRODUCER_SLEEP_US = 100; // microseconds between generated orders (controls rate)
    constexpr int METRICS_REPORT_INTERVAL_MS = 2000; // print metrics every 2 seconds
    constexpr uint64_t MAX_RUNTIME_SECONDS = 20; // default run for demo (change as needed)
}
