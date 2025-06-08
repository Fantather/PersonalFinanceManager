#include "CreditCard.h"

void CreditCard::deposit(double amount)
{
	if (amount <= 0) return;

	int final_amount = amount - current_debt_;

    // We extinguish the debt
    if (current_debt_ > 0) {
        double payment = std::min(amount, current_debt_);
        current_debt_ -= payment;
        amount -= payment;
    }

    // The remaining - for a positive balance
    if (amount > 0) {
        balance_ += amount;
    }
}

bool CreditCard::withdraw(double amount)
{
    if (amount >= 0) return false;

    double avaliable = balance_ + (credit_limit_ - current_debt_);

    // If we exceed the limit, we reject the transaction
    if (amount > avaliable) return false;

    // If there is enough money
    if (amount <= balance_)
    {
        balance_ -= amount;
    }

    // Knowing Balance and the rest goes in debt
    else
    {
        double over = amount - balance_;
        balance_ = 0.0;
        current_debt_ += over;
    }

    return true;
}

// I transfer the annual rate to the monthly and accrue if there is a debt
void CreditCard::accrue_interest()
{
    if (current_debt_ <= 0) return;
    double mounthly_rate = interest_rate_ / 12.0;
    current_debt_ += current_debt_ * mounthly_rate;
}