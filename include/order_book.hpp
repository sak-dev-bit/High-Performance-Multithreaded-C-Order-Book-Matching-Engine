#pragma once
#include "order.hpp"
#include <map>
#include <list>
#include <mutex>
#include <vector>
#include <cstdint>

struct Match {
    uint64_t buy_order_id;
    uint64_t sell_order_id;
    double price;
    uint64_t quantity;
};

class OrderBook {
public:
    OrderBook();

    void add_order(const Order &order);
    std::vector<Match> process_order(const Order &order);
    // returns matches produced by adding this order. The unmatched remainder (if any) is added to book by add_order inside process_order

    uint64_t total_matches_executed() const;

private:
    // price -> list of Orders (FIFO)
    std::map<double, std::list<Order>, std::greater<double>> bids; // highest price first
    std::map<double, std::list<Order>, std::less<double>> asks;    // lowest price first

    mutable std::mutex mtx;
    uint64_t matches_executed;
};
