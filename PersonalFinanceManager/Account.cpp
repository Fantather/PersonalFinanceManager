#include "Account.h"
#include "CreditCard.h"

// Default value for balance
const double Account::default_initial_balance = 0.0;

// Constructor
Account::Account(const int id, const std::string& name, double initial_balance) : id_(id), name_(name), balance_(initial_balance) {}

void Account::deposit(double amount)
{
	if (amount < 0)
	{
		return;
	}

	balance_ += amount;
}

bool Account::withdraw(double amount)
{
	if(amount > balance_)
	{
		return false;
	}

	else
	{
		balance_ -= amount;
		return true;
	}
}


