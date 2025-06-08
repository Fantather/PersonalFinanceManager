#pragma once
#include <vector>
#include <map>
#include <unordered_map>
#include <memory>
#include <chrono>

#include "Account.h"
#include "Transaction.h"
#include "Category.h"


class FinanceManager {
private:
	// Free IDs that will be assigned to objects
	int next_account_id_	 = 1;
	int next_category_id_	 = 1;
	int next_transaction_id_ = 1;

	std::unordered_map<int, std::unique_ptr<Account>>	accounts_;		// Wallets and Bank Cards
	std::unordered_map<int, std::unique_ptr<Category>>	category_;		// Categories of expenses
	std::multimap<int, std::unique_ptr<Transaction>>	transactions_;	// The history of transactions
};