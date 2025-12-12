#include "include/order_book.hpp"
#include <algorithm>

OrderBook::OrderBook(): matches_executed(0) {}

void OrderBook::add_order(const Order &order) {
    std::lock_guard<std::mutex> lk(mtx);
    if (order.side == Side::BUY) {
        bids[order.price].push_back(order);
    } else {
        asks[order.price].push_back(order);
    }
}

std::vector<Match> OrderBook::process_order(const Order &incoming) {
    std::vector<Match> matches;
    std::lock_guard<std::mutex> lk(mtx);

    Order order = incoming; // make local mutable copy

    if (order.side == Side::BUY) {
        // Try match against the lowest asks while price condition holds
        while (order.quantity > 0 && !asks.empty()) {
            auto it = asks.begin(); // lowest ask price
            double ask_price = it->first;
            if (order.price < ask_price) break; // cannot match
            auto &list = it->second;
            while (!list.empty() && order.quantity > 0) {
                Order &ask_order = list.front();
                uint64_t traded_qty = std::min(order.quantity, ask_order.quantity);
                Match m{ order.id, ask_order.id, ask_price, traded_qty };
                matches.push_back(m);
                matches_executed++;
                // update quantities
                order.quantity -= traded_qty;
                ask_order.quantity -= traded_qty;
                if (ask_order.quantity == 0) list.pop_front();
            }
            if (list.empty()) asks.erase(it);
        }
        // if remaining quantity > 0, add to bids
        if (order.quantity > 0) {
            bids[order.price].push_back(order);
        }
    } else {
        // SELL incoming -> match against highest bids
        while (order.quantity > 0 && !bids.empty()) {
            auto it = bids.begin(); // highest bid price (map with greater comparator)
            double bid_price = it->first;
            if (order.price > bid_price) break; // cannot match
            auto &list = it->second;
            while (!list.empty() && order.quantity > 0) {
                Order &bid_order = list.front();
                uint64_t traded_qty = std::min(order.quantity, bid_order.quantity);
                Match m{ bid_order.id, order.id, bid_price, traded_qty };
                matches.push_back(m);
                matches_executed++;
                order.quantity -= traded_qty;
                bid_order.quantity -= traded_qty;
                if (bid_order.quantity == 0) list.pop_front();
            }
            if (list.empty()) bids.erase(it);
        }
        if (order.quantity > 0) {
            asks[order.price].push_back(order);
        }
    }

    return matches;
}

uint64_t OrderBook::total_matches_executed() const {
    std::lock_guard<std::mutex> lk(mtx);
    return matches_executed;
}
