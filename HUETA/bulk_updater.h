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
    double proc_ = 1.0;
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
        add_.delta = (add_.delta * tax_.proc_) + other.add_.delta;
        spnd_.delta = spnd_.delta + other.spnd_.delta;
    }

    Money Collapse(Money origin, IndexSegment segment) const {
        Money res;
        res.earn_ = (origin.earn_ * tax_.proc_) + add_.delta * static_cast<double>(segment.length());
        res.spend_ = origin.spend_ + spnd_.delta * static_cast<double>(segment.length());
        return res;
    }

private:
    // apply tax first, then add
    BulkTaxApplier tax_;
    BulkMoneyAdder add_;
    BulkMoneySpender spnd_;
};
