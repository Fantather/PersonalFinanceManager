#pragma once
#include <algorithm>
#include "BankCard.h"

// Credit card with limit, debt, interest
class CreditCard : public BankCard {
public:
    static const double default_current_debt;

protected:
    double credit_limit_;    // Maximum credit
    double current_debt_;    // Outstanding debt
    double interest_rate_;   // Annual interest rate

public:
    CreditCard(int id,
        const std::string& name,
        const std::string& card_number,
        const std::string& holder_name,
        const std::string& expiration_date,
        double credit_limit,
        double interest_rate,
        double initial_balance = default_initial_balance)
        : BankCard(id, name, card_number, holder_name, expiration_date, initial_balance),
        credit_limit_(credit_limit > 0.0 ? credit_limit : 0.0),
        current_debt_(0.0),
        interest_rate_(interest_rate) {}

    ~CreditCard() override = default;

    // Override deposit / withdraw
    void  deposit(double amount) override;
    bool  withdraw(double amount) override;

    // Getters
    double get_credit_limit()  const { return credit_limit_; }
    double get_current_debt()  const { return current_debt_; }
    double get_interest_rate() const { return interest_rate_; }

    // Setter for debt (used in loading)
    void set_current_debt(double debt) { current_debt_ = debt; }

    // Accrue monthly interest on outstanding debt
    void accrue_interest();
};