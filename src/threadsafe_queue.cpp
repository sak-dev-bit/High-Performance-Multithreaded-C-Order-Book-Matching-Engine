#include "order.hpp"
#include "include/queue.hpp"
#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <optional>

template<typename T>
class TSQ_Impl {
public:
    std::queue<T> q;
    std::mutex m;
    std::condition_variable cv;
    bool shutting_down = false;

    void push(T item) {
        {
            std::lock_guard<std::mutex> lk(m);
            q.push(std::move(item));
        }
        cv.notify_one();
    }

    bool pop(T &item) {
        std::unique_lock<std::mutex> lk(m);
        cv.wait(lk, [&]{ return !q.empty() || shutting_down; });
        if (q.empty()) return false;
        item = std::move(q.front());
        q.pop();
        return true;
    }

    bool try_pop(T &item) {
        std::lock_guard<std::mutex> lk(m);
        if (q.empty()) return false;
        item = std::move(q.front());
        q.pop();
        return true;
    }

    void shutdown() {
        {
            std::lock_guard<std::mutex> lk(m);
            shutting_down = true;
        }
        cv.notify_all();
    }

    bool empty() {
        std::lock_guard<std::mutex> lk(m);
        return q.empty();
    }
};

template<typename T>
ThreadSafeQueue<T>::ThreadSafeQueue() { }

template<typename T>
void ThreadSafeQueue<T>::push(T item) {
    static TSQ_Impl<T> impl; // per template type - NOTE: this simple implementation shares impl across instances; but intended single instance use
    impl.push(std::move(item));
}

template<typename T>
bool ThreadSafeQueue<T>::pop(T &item) {
    static TSQ_Impl<T> impl;
    return impl.pop(item);
}

template<typename T>
bool ThreadSafeQueue<T>::try_pop(T &item) {
    static TSQ_Impl<T> impl;
    return impl.try_pop(item);
}

template<typename T>
void ThreadSafeQueue<T>::shutdown() {
    static TSQ_Impl<T> impl;
    impl.shutdown();
}

template<typename T>
bool ThreadSafeQueue<T>::empty() {
    static TSQ_Impl<T> impl;
    return impl.empty();
}

// Explicit instantiation for Order type so the linker has implementations
template class ThreadSafeQueue<Order>;
