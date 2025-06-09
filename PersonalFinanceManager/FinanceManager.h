#pragma once
#include <vector>
#include <map>
#include <unordered_map>
#include <memory>
#include <chrono>
//#include <boost/multi_index_container.hpp>
//#include <boost/multi_index/ordered_index.hpp>        // Не смог подключить :D
//#include <boost/multi_index/composite_key.hpp>

#include "Account.h"
#include "Transaction.h"
#include "Category.h"
#include "Report.h"
#include "ReportStorage.h"

class FileStorage; // forward

class FinanceManager {
private:
    friend class FileStorage;

	// Free IDs that will be assigned to new objects
	int next_account_id_	 = 1;
	int next_category_id_	 = 1;
	int next_transaction_id_ = 1;

	std::unordered_map<int, std::unique_ptr<Account>>	accounts_;		// Wallets and Bank Cards
	std::unordered_map<int, std::unique_ptr<Category>>	category_;		// Categories of expenses
	std::vector<std::unique_ptr<Transaction>>			transactions_;	// The history of transactions

    std::unique_ptr<Report> create_report(const std::chrono::system_clock::time_point& start, const std::chrono::system_clock::time_point& end);

public:
	FinanceManager() = default;
	~FinanceManager() = default;

    int add_wallet(const std::string& name, double initial_balance = 0.0);
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

    bool remove_account(int account_id);  // returns True with successful removal

    // Getter
    Account* get_account(int account_id); // returns nullptr if not found

    /*--- Work with categories ---*/
    int add_category(const std::string& name, int parentId = -1);
    bool remove_category(int category_id);
    Category* get_category(int category_id);

    /*--- Transaction management ---*/

    // Add income
    bool add_income(int account_id, double amount, const std::chrono::system_clock::time_point& timestamp = std::chrono::system_clock::now());
    // Add a category
    bool add_expense(int account_id, double amount, int category_id, const std::chrono::system_clock::time_point& timestamp = std::chrono::system_clock::now());

    /*--- Get a complete list of transactions (for reports) ---*/
    const std::vector<std::unique_ptr<Transaction>>& get_all_transactions() const;

    /*--- Methods for forming reports and ratings ---*/
    std::vector<std::unique_ptr<Report>> generate_daily_report(const std::chrono::system_clock::time_point& date);
    std::vector<std::unique_ptr<Report>> generate_weekly_report(const std::chrono::system_clock::time_point& week_start);
    std::vector<std::unique_ptr<Report>> generate_monthly_report(int year, int month);

    /*--- Persistence of reports & ratings via ReportStorage ---*/
    bool save_reports(const std::vector<std::unique_ptr<Report>>& reports,
        const std::vector<ReportStorage::PeriodType>& types,
        const std::string& filename);


    bool load_reports(std::vector<std::unique_ptr<Report>>& reports,
        std::vector<ReportStorage::PeriodType>& types,
        const std::string& filename);
};