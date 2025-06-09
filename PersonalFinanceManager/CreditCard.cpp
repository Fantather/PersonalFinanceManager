#include "CreditCard.h"

const double CreditCard::default_current_debt = 0.0;

void CreditCard::deposit(double amount)
{
    if (amount <= 0.0) return;

    // First pay down debt
    double payment = std::min(amount, current_debt_);
    current_debt_ -= payment;
    amount -= payment;

    // Remainder goes to balance
    if (amount > 0.0) {
        balance_ += amount;
    }
}

bool CreditCard::withdraw(double amount)
{
    if (amount <= 0.0) return false;

    double available = balance_ + (credit_limit_ - current_debt_);
    if (amount > available) return false;

    if (amount <= balance_) {
        balance_ -= amount;
    }
    else {
        double over = amount - balance_;
        balance_ = 0.0;
        current_debt_ += over;
    }
    return true;
}

void CreditCard::accrue_interest()
{
    if (current_debt_ <= 0.0) return;
    double monthly_rate = interest_rate_ / 12.0;
    current_debt_ += current_debt_ * monthly_rate;
}