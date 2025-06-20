#pragma once
#include "Transaction.h"

// At the consumption, the system should automatically call WhDraw (amount) the specified Account. If you do not have enough funds - return the error / rollback of the operation
// Tied to the category of Transaction for reports and ratings

class Expence : public Transaction {
private:
	int category_id_;	// ID of cost categories

public:
	Expence(const double amount, const int id, const int account_id, const std::chrono::system_clock::time_point& timestamp, const int category_id)
		: Transaction(amount, id, account_id, timestamp), category_id_(category_id) {}
	~Expence() override = default;

	// Getter
	int get_category_id() const override { return category_id_; }

	bool is_expence() const override { return true; }
};