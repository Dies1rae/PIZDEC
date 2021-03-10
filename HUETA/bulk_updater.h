#pragma once

#include "entities.h"
#include "summing_segment_tree.h"

#include <cstdint>
#include <cmath>

struct BulkMoneyAdder {
    double delta = {};
};

struct BulkMoneySpender {
    double delta = {};
};

struct BulkTaxApplier {
    double ComputeFactor() const {
        static const double factor = 0.87;
        return std::pow(factor, count);
    }

    int count = 0;
};

class BulkLinearUpdater {
public:
    BulkLinearUpdater() = default;

    BulkLinearUpdater(const BulkMoneyAdder& add)
        : add_(add) {
    }

    BulkLinearUpdater(const BulkMoneySpender& spnd)
        : spnd_(spnd) {
    }

    BulkLinearUpdater(const BulkTaxApplier& tax)
        : tax_(tax) {
    }

    void CombineWith(const BulkLinearUpdater& other) {
        tax_.count += other.tax_.count;
        add_.delta = add_.delta * other.tax_.ComputeFactor() + other.add_.delta;
        spnd_.delta = spnd_.delta + other.spnd_.delta;
    }

    double Collapse(double origin, IndexSegment segment) const {
        return (origin * tax_.ComputeFactor() + add_.delta * static_cast<double>(segment.length())) - spnd_.delta * static_cast<double>(segment.length());
    }

private:
    // apply tax first, then add
    BulkTaxApplier tax_;
    BulkMoneyAdder add_;
    BulkMoneySpender spnd_;
};
