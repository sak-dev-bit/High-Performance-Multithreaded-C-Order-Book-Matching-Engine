#pragma once
#include <cstdint>
#include <chrono>
#include <string>

enum class Side { BUY, SELL };

struct Order {
    uint64_t id;
    Side side;
    double price;
    uint64_t quantity;
    std::chrono::high_resolution_clock::time_point created_ts;

    Order() = default;
    Order(uint64_t id_, Side side_, double price_, uint64_t qty_)
        : id(id_), side(side_), price(price_), quantity(qty_), created_ts(std::chrono::high_resolution_clock::now()) {}

    std::string side_str() const { return side == Side::BUY ? "BUY" : "SELL"; }
};
