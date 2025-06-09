#include "Account.h"

// Default initial balance
const double Account::default_initial_balance = 0.0;

// Constructor
Account::Account(int id, const std::string& name, double initial_balance)
    : id_(id), name_(name), balance_(initial_balance) {}

// Replenish the bill
void Account::deposit(double amount)
{
    if (amount <= 0.0) return;
    balance_ += amount;
}

// Withdraw money
bool Account::withdraw(double amount)
{
    if (amount > balance_) return false;
    balance_ -= amount;
    return true;
}