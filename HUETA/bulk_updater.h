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
    double proc_ = {};
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
        add_.delta = add_.delta * ((100 - other.tax_.proc_) / 100) + other.add_.delta;
        spnd_.delta = spnd_.delta + other.spnd_.delta;
    }

    double Collapse(double origin, IndexSegment segment) const {
        return (origin * ((100 - tax_.proc_) / 100) + add_.delta * static_cast<double>(segment.length())) - spnd_.delta * static_cast<double>(segment.length());
    }

private:
    // apply tax first, then add
    BulkTaxApplier tax_;
    BulkMoneyAdder add_;
    BulkMoneySpender spnd_;
};
