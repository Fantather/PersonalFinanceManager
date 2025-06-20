#include "CreditCard.h"

bool CreditCard::deposit(double amount)
{
	if (amount < 0.0)
		return false;

	double payment = std::min(amount, current_debt_);
	amount -= payment;
	current_debt_ -= payment;

	if (amount > 0.0)
	{
		balance_ += amount;
	}

	return true;
}

bool CreditCard::withdraw(const double amount)
{
	if (amount < 0.0)
		return false;


	double avaliable = amount + credit_limit_;		// The maximum possible amount of money to withdraw
	if (amount > avaliable)
		return false;


	if (amount <= balance_)
	{
		balance_ -= amount;
	}

	else
	{
		double over = amount - balance_;
		balance_ = 0.0;
		current_debt_ += over;
	}

	return true;
}

void CreditCard::accrue_interest()
{
	if (current_debt_ <= 0.0)
		return;

	double monthly_rate = interest_rate_ / 12.0;
	current_debt_ += current_debt_ * monthly_rate;
}
