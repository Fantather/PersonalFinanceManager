#pragma once
#include <vector>
#include <map>
#include <unordered_map>
#include <memory>
#include <chrono>

#include <boost/multi_index_container.hpp>
#include <boost/multi_index_container_fwd.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/hashed_index.hpp>		// Всё-таки подключил со второй попытки :D
#include <boost/multi_index/identity.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/mem_fun.hpp>

#include "Account.h"
#include "Category.h"
#include "Report.h"
#include "ReportStorage.h"
#include "TransactionMultiIndex.h"
#include "DebitCard.h"
#include "CreditCard.h"
#include "Wallet.h"
#include "Transaction.h"
#include "Income.h"
#include "Expence.h"

namespace bmi = boost::multi_index;
// namespace time_ = std::chrono::system_clock::time_point;


class FileStorage; // forward

class FinanceManager {
private:
    friend class FileStorage;

	// Free IDs that will be assigned to new objects
	int next_account_id_	 = 1;
	int next_category_id_	 = 1;
	int next_transaction_id_ = 1;

	std::unordered_map<int, std::unique_ptr<Account>>	accounts_;			// Wallets and Bank Cards
	std::unordered_map<int, std::unique_ptr<Category>>	category_;			// Categories of expenses
	TransactionMultiIndex transactions_;									// The history of transactions

public:
	FinanceManager() = default;
	~FinanceManager() = default;

	/*--- Work with Cards ---*/
	int add_debitCard(const std::string& name,
		const std::string& card_Number,
		const std::string& holder_Name,
		const std::string& expiration_Date,
		double initial_balance = 0.0);

	int add_creditCard(const std::string& name,
		const std::string& card_number,
		const std::string& holder_name,
		const std::string& expiration_date,
		double credit_limit,
		double interest_rate,
		double initial_balance = 0.0);

	int add_wallet(const std::string& name, double initial_balance = 0.0);

	bool remove_account(int account_id);							// returns True with successful removal
	Account* get_account(const int account_id);						// returns nullptr if not found


	/*--- Work with Categories ---*/
	int add_category(const std::string& name, const int parentId = -1);
	//bool remove_category(const int category_id);					// returns false if not found
	Category* get_category(const int category_id);					// returns nullptr if not found		


	/*--- Work with Transactions ---*/
	bool add_income(const int account_id, const double amount, const std::chrono::system_clock::time_point& timestamp = std::chrono::system_clock::now());
	bool add_expense(const int account_id, const double amount, const int category_id, const std::chrono::system_clock::time_point& timestamp = std::chrono::system_clock::now());

	/*--- Work with Reports ---*/

	// Helper methods
	std::vector<std::shared_ptr<const Transaction>> create_report(const std::chrono::system_clock::time_point& start, const std::chrono::system_clock::time_point& end);
	std::unique_ptr<Report> build_report(const std::chrono::system_clock::time_point& start, const std::chrono::system_clock::time_point& end);

	// Generate reports
	std::vector<std::unique_ptr<Report>> generate_daily_report(const std::chrono::system_clock::time_point& date);
	std::vector<std::unique_ptr<Report>> generate_weekly_report(const std::chrono::system_clock::time_point& week_start);
	std::vector<std::unique_ptr<Report>> generate_monthly_report(int year, int month);
};

