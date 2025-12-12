#pragma once
#include <memory>
#include "order.hpp"

template<typename T>
class ThreadSafeQueue {
public:
    ThreadSafeQueue() = default;
    ~ThreadSafeQueue() = default;

    void push(T item);
    bool pop(T &item); // blocking pop
    bool try_pop(T &item); // non-blocking
    void shutdown(); // notify waiting threads
    bool empty();
};
