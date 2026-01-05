
# High-Performance Multithreaded C Order Book Matching Engine

A **high-performance, multithreaded C++ order book matching engine** prototype designed to simulate continuous trade matching under concurrent load. The engine processes producer-generated orders via a thread-safe queue and executes matching logic based on price-time priority, printing latency metrics and trade events. ([GitHub][1])

---

## 🚀 Project Overview

**Purpose:** Provides a proof-of-concept matching engine core capable of concurrently handling buy/sell orders and matching them efficiently.
**Core Value:** Demonstrates a basic multithreaded architecture for order ingestion and matching with latency metrics.
**Language:** Modern C++ (headers in `include/`, implementation in `src/`).
**Build System:** Simple Makefile-based workflow. ([GitHub][1])

---

## 🔍 Features

* **Multithreaded producer–consumer architecture**
* **Thread-safe queue** for order buffering
* **Matching engine logic with price-time priority**
* **Latency measurement and periodic metric reporting**
* **Extensible codebase for benchmarking and improvements** ([GitHub][1])

---

## 📦 Getting Started

### Prerequisites

Make sure you have the following installed:

* A C++ compiler supporting C++11 or later
* `make` build tool (GNU Make or equivalent)
* Standard development environment (Linux/Mac/WSL recommended)

```bash
# Confirm compiler availability
g++ --version

# Confirm make availability
make --version
```

---

## 🛠️ Installation & Build

From the repository root:

```sh
# Clone the repo (if not already)
git clone https://github.com/sak-dev-bit/High-Performance-Multithreaded-C-Order-Book-Matching-Engine.git
cd High-Performance-Multithreaded-C-Order-Book-Matching-Engine

# Build the project
make
```

The build will produce an executable according to the Makefile configuration. ([GitHub][1])

---

## 📁 Project Structure

```
.
├── include/              # Header files
│   ├── config.hpp        # Engine configuration
│   ├── metrics.hpp       # Performance metrics
│   ├── order.hpp         # Order representation
│   ├── order_book.hpp    # Core order book interface
│   └── queue.hpp         # Thread-safe queue abstraction
├── src/                  # Source files
│   ├── producer.cpp      # Order producer implementation
│   ├── consumer.cpp      # Consumer/worker logic
│   ├── matching.cpp      # Matching engine logic
│   ├── queue.cpp         # Queue implementation
│   └── main.cpp          # Entry point
├── Makefile              # Build instructions
└── README.md             # Project documentation
```

* **include/**: API surface and configurations.
* **src/**: Functional implementation including producer, consumer, matching logic.
* **Makefile**: Compilation instructions. ([GitHub][1])

---

## ▶️ Usage Guide

Once built, run the engine executable:

```sh
./order_book_engine
```

By default:

* The **producer thread** generates random orders at a rate controlled by `PRODUCER_SLEEP_US` in `config.hpp`.
* The **consumer** pulls orders from the thread-safe queue, processes them, and runs matching logic.
* Metrics and trades print to `stdout`. ([GitHub][1])

### Key Concepts

**Order Producer:** Simulates market order flow.
**Order Consumer:** Matches orders using FIFO and price-time priority.
**Thread-safe Queue:** Ensures safe concurrent access between producer and consumer threads.
**Metrics Logging:** Outputs processing latency and performance metrics. ([GitHub][1])

---

## 🧪 Developer Operations

### Configuration

Adjust runtime behavior in `include/config.hpp`:

```cpp
// Example tunables
static constexpr size_t MAX_RUNTIME_SECONDS = 20;
static constexpr size_t PRODUCER_SLEEP_US   = 1000;
```

### Extend & Improve

Suggested improvements:

* Add **unit tests** and a test harness.
* Introduce a **lock-free queue** for further latency reduction.
* Integrate a **benchmark suite** under `benchmarks/`.
* Replace stdout logging with **spdlog** or similar. ([GitHub][1])

---

## 🤝 Contributing

Contributions are welcome. Follow general GitHub guidelines:

1. Fork the repository
2. Create a feature branch: `git checkout -b feature/foo`
3. Commit your changes
4. Open a pull request

**Note:** Add meaningful commit messages and tests where applicable.

---

## 📄 License

*No license file present.* You should add a valid open-source license (e.g., MIT, Apache-2.0) to clarify reuse permissions.

---

## 🛠️ Support & Troubleshooting

If you encounter build errors:

* Verify your compiler version supports modern C++ features.
* Ensure includes and library paths are correct.
* Use `make clean` then `make` to rebuild.

For advanced debugging:

```sh
g++ -std=c++17 -g src/*.cpp -o debug_engine
```
tHub - sak-dev-bit/High-Performance-Multithreaded-C-Order-Book-Matching-Engine"
