#pragma once
#include "Account.h"

// Simple wallet (cash or virtual), no extra constraints
class Wallet : public Account {
public:
    Wallet(int id, const std::string& name, double initial_balance = default_initial_balance)
        : Account(id, name, initial_balance) {}
    ~Wallet() override = default;

    // Uses base deposit/withdraw behavior
};