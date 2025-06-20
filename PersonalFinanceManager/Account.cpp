#include "Account.h"

bool Account::deposit(const double amount)
{
	if (amount < 0)
		return false;

	balance_ += amount;
	return true;
}

bool Account::withdraw(const double amount)
{
	if (amount < 0 || amount > balance_)
		return false;

	balance_ -= amount;
	return true;
}
