#pragma once
#include "Transaction.h"

// Class for expenses
// At the consumption, the system should automatically call WhDraw (amount) the specified Account. If you do not have enough funds - return the error / rollback of the operation
// Tied to the category of Transaction for reports and ratings

class Expence : public Transaction {
private:
	int category_id_;	// ID of cost categories

public:
	Expence(int account_id, double amount, const std::chrono::system_clock::time_point& timestamp, int id, int category_id)
		: Transaction(account_id, amount, timestamp, id)
	{
		category_id_ = category_id;
	}
	~Expence() = default;

	int get_category_id() const { return category_id_; }

	bool is_expense() const override { return true; }
};