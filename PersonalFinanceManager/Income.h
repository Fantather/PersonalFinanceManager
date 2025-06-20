#pragma once
#include "Transaction.h"

// Is responsible for replenishing money to the account / card / wallet
// When registering "replenishment", it automatically calls deposit (amount) in the corresponding Account

class Income : public Transaction {
public:
	using Transaction::Transaction;
	~Income() override = default;

	bool is_expence() const override { return false; }
};