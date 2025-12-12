
## Files
- `include/` : headers (`order.hpp`, `order_book.hpp`, `queue.hpp`, `metrics.hpp`, `config.hpp`)
- `src/` : source files implementing producer, consumer, order book, queue, metrics and logging.
- `Makefile` : build instructions.

## Run
The program runs for `cfg::MAX_RUNTIME_SECONDS` (default 20s). Adjust this in `include/config.hpp`.

## Behavior
- The producer pushes random orders to the thread-safe queue at a rate controlled by `PRODUCER_SLEEP_US`.
- The consumer pops orders, measures latency, and processes matching logic (price-time priority).
- If a trade occurs, it's logged to stdout.
- Periodic metrics are printed to stdout.

## Extending
- Add proper logging (spdlog), benchmarking harness in `benchmarks/`, unit tests in `tests/`.
- Swap `ThreadSafeQueue` with a lock-free queue for latency improvements.
"# High-Performance-Multithreaded-C-Order-Book-Matching-Engine" 
