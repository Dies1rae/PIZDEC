#pragma once

class Money {
public:
    double earn_ = 0.0;
    double spend_ = 0.0;
};

template <typename Data>
Data operator+(const Data& lhs, const Data& rhs) {
    Data res;
    res.earn_ = lhs.earn_ + rhs.earn_;
    res.spend_ = lhs.spend_ + rhs.spend_;
    return res;
}
