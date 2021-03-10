#include "parser.h"

using namespace std;

namespace queries {
    class ComputeIncome : public ComputeQuery {
    public:
        using ComputeQuery::ComputeQuery;
        ReadResult Process(const BudgetManager& budget) const override {
            return { budget.ComputeSum(GetFrom(), GetTo()) };
        }

        class Factory : public QueryFactory {
        public:
            std::unique_ptr<Query> Construct(std::string_view config) const override {
                auto parts = Split(config, ' ');
                return std::make_unique<ComputeIncome>(Date::FromString(parts[0]), Date::FromString(parts[1]));
            }
        };
    };

    class Alter : public ModifyQuery {
    public:
        Alter(Date from, Date to, double amount, bool A_S)
            : ModifyQuery(from, to)
            , amount_(amount)
            , operation_(A_S){
        }

        void Process(BudgetManager& budget) const override {
            double day_income = amount_ / (Date::ComputeDistance(GetFrom(), GetTo()) + 1);
            if (operation_) {
                budget.AddBulkOperation(GetFrom(), GetTo(), BulkMoneyAdder{ day_income });
            } else {
                budget.AddBulkOperation(GetFrom(), GetTo(), BulkMoneySpender{ day_income });
            }
        }

        class Factory_earn : public QueryFactory {
        public:
            std::unique_ptr<Query> Construct(std::string_view config) const override {
                auto parts = Split(config, ' ');
                double payload = std::stod(std::string(parts[2]));
                return std::make_unique<Alter>(Date::FromString(parts[0]), Date::FromString(parts[1]), payload, 1);
            }
        };
        class Factory_spend : public QueryFactory {
        public:
            std::unique_ptr<Query> Construct(std::string_view config) const override {
                auto parts = Split(config, ' ');
                double payload = std::stod(std::string(parts[2]));
                return std::make_unique<Alter>(Date::FromString(parts[0]), Date::FromString(parts[1]), payload, 0);
            }
        };

    private:
        bool operation_;
        double amount_;
    };

    class PayTax : public ModifyQuery {
    public:
        PayTax(Date from, Date to, double taxes_proc)
            : ModifyQuery(from, to)
            , taxes_proc_(taxes_proc){
        }

        void Process(BudgetManager& budget) const override {
            budget.AddBulkOperation(GetFrom(), GetTo(), BulkTaxApplier{ taxes_proc_ });
        }

        class Factory : public QueryFactory {
        public:
            std::unique_ptr<Query> Construct(std::string_view config) const override {
                auto parts = Split(config, ' ');
                double tax_proc = std::stod(std::string(parts[2]));
                return std::make_unique<PayTax>(Date::FromString(parts[0]), Date::FromString(parts[1]), tax_proc);
            }
        };

    private:
        double taxes_proc_;
    };

}  // namespace queries

const QueryFactory& QueryFactory::GetFactory(std::string_view id) {
    static queries::ComputeIncome::Factory compute_income;
    static queries::Alter::Factory_earn earn;
    static queries::Alter::Factory_spend spend;
    static queries::PayTax::Factory pay_tax;
    static std::unordered_map<std::string_view, const QueryFactory&> factories
        = { {"ComputeIncome"sv, compute_income}, {"Earn"sv, earn}, {"Spend"sv, spend}, {"PayTax"sv, pay_tax} };

    return factories.at(id);
}
