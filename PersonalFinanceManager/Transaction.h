#pragma once
#include <chrono>

// Abstract base for all transactions: timestamp, amount, account
class Transaction {
protected:
	std::chrono::system_clock::time_point timestamp_;	// Transaction time
	double amount_;			// The amount of the transaction
	int transaction_id_;	// Unique ID transaction
	int account_id_;		// Unique ID account of the transaction

public:
	// Constructor and Destructor
	Transaction(const double amount, const int id, const int account_id, const std::chrono::system_clock::time_point& timestamp = std::chrono::system_clock::now())
		: timestamp_(timestamp), amount_(amount), transaction_id_(id), account_id_(account_id) {}
	virtual ~Transaction() = default;

	// Getters
	const std::chrono::system_clock::time_point& get_timestamp() const { return timestamp_; }
	double get_amount() const { return amount_; }
	int get_transaction_id() const { return transaction_id_; }
	int get_account_id() const { return account_id_; }

	// To make the class abstract and distinguish Income vs Expense
	virtual int get_category_id() const { return -1; }
	virtual bool is_expence() const { return false; }
};