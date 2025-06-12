#include "FinanceManager.h"
#include "Wallet.h"
#include "DebitCard.h"
#include "CreditCard.h"
#include "Income.h"
#include "Expence.h"
#include "ReportStorage.h"

#include <algorithm>
#include <chrono>

using namespace std::chrono;


// Account management
int FinanceManager::add_wallet(const std::string& name, double initial_balance) {
    int id = next_account_id_++;
    accounts_[id] = std::make_unique<Wallet>(id, name, initial_balance);
    return id;
}

int FinanceManager::add_debitCard(const std::string& name,
    const std::string& card_Number,
    const std::string& holder_Name,
    const std::string& expiration_Date,
    double initial_balance)
{
    int id = next_account_id_++;
    accounts_[id] = std::make_unique<DebitCard>(id, name, card_Number, holder_Name, expiration_Date, initial_balance);
    return id;
}

int FinanceManager::add_creditCard(const std::string& name,
    const std::string& card_number,
    const std::string& holder_name,
    const std::string& expiration_date,
    double credit_limit,
    double interest_rate,
    double initial_balance)
{
    int id = next_account_id_++;
    accounts_[id] = std::make_unique<CreditCard>(id, name, card_number, holder_name, expiration_date, credit_limit, interest_rate, initial_balance);
    return id;
}

bool FinanceManager::remove_account(int account_id) {
    return accounts_.erase(account_id) > 0;
}

Account* FinanceManager::get_account(int account_id) {
    auto it = accounts_.find(account_id);
    return it != accounts_.end() ? it->second.get() : nullptr;
}

// Category management
int FinanceManager::add_category(const std::string& name, int parentId) {
    int id = next_category_id_++;
    category_[id] = std::make_unique<Category>(id, name, parentId);
    return id;
}

bool FinanceManager::remove_category(int category_id) {
    return category_.erase(category_id) > 0;
}

Category* FinanceManager::get_category(int category_id) {
    auto it = category_.find(category_id);
    return it != category_.end() ? it->second.get() : nullptr;
}

// Transaction management
bool FinanceManager::add_income(int account_id, double amount, const std::chrono::system_clock::time_point& timestamp) {
    Account* acc = get_account(account_id);
    if (!acc || amount <= 0) return false;
    acc->deposit(amount);

    int id = next_transaction_id_++;

    transactions_.push_back(std::make_unique<Income>(account_id, amount, timestamp, id));
    return true;
}

bool FinanceManager::add_expense(int account_id, double amount, int category_id, const std::chrono::system_clock::time_point& timestamp) {
    Account* acc = get_account(account_id);
    if (!acc || amount <= 0) return false;
    if (!acc->withdraw(amount)) return false;

    int id = next_transaction_id_++;

    transactions_.push_back(std::make_unique<Expence>(account_id, amount, timestamp, id, category_id));
    return true;
}

const std::vector<std::unique_ptr<Transaction>>& FinanceManager::get_all_transactions() const {
    return transactions_;
}

// Report generation helpers
static double sum_by_type(const std::vector<std::unique_ptr<Transaction>>& txs,
    bool expense, const std::chrono::system_clock::time_point& start,
    const std::chrono::system_clock::time_point& end,
    std::map<int, double>& by_category,
    std::vector<std::pair<int, double>>& top_tx)
{
    double total = 0.0;
    for (const auto& up : txs) {
        auto t = up.get();
        if (t->is_expense() != expense) continue;
        auto ts = t->get_timestamp();
        if (ts < start || ts >= end) continue;
        double amt = t->get_amount();
        total += amt;
        if (expense) {
            // cast to expense
            auto exp = static_cast<Expence*>(t);
            by_category[exp->get_category_id()] += amt;
        }
        top_tx.emplace_back(t->get_id(), amt);
    }
    return total;
}

// Helper: calculates total income or expenses, collects category sums and transaction list
static double sum_and_collect(
    const std::vector<std::unique_ptr<Transaction>>& txs,
    bool expense,
    const std::chrono::system_clock::time_point& start,
    const std::chrono::system_clock::time_point& end,
    std::map<int, double>& by_category,
    std::vector<std::pair<int, double>>& tx_list)
{
    double total = 0.0;
    for (const auto& up : txs) {
        const Transaction* t = up.get();
        if (t->is_expense() != expense) continue;

        auto ts = t->get_timestamp();
        if (ts < start || ts >= end) continue;

        double amt = t->get_amount();
        total += amt;

        if (expense) {
            // Cast to Expence and record category sum
            int category = static_cast<const Expence*>(t)->get_category_id();
            by_category[category] += amt;
        }

        // Store transaction ID and amount
        tx_list.emplace_back(t->get_id(), amt);
    }
    return total;
}

// Creates a single report over the given time interval
std::unique_ptr<Report> FinanceManager::create_report(
    const std::chrono::system_clock::time_point& start,
    const std::chrono::system_clock::time_point& end)
{
    auto report = std::make_unique<Report>();
    report->period_start = start;
    report->period_end = end;

    std::map<int, double> category_totals;
    std::vector<std::pair<int, double>> transaction_list;

    // Collect income and expenses separately
    report->total_income = sum_and_collect(transactions_, false, start, end, category_totals, transaction_list);
    report->total_expenses = sum_and_collect(transactions_, true, start, end, category_totals, transaction_list);

    report->expenses_by_category = category_totals;

    // Top 3 transactions (by amount)
    std::sort(transaction_list.begin(), transaction_list.end(),
        [](const auto& a, const auto& b) { return a.second > b.second; });

    for (size_t i = 0; i < std::min<size_t>(3, transaction_list.size()); ++i)
        report->top3_transactions.push_back(transaction_list[i]);

    // Top 3 categories (by total expenses)
    std::vector<std::pair<int, double>> sorted_categories(
        report->expenses_by_category.begin(), report->expenses_by_category.end());

    std::sort(sorted_categories.begin(), sorted_categories.end(),
        [](const auto& a, const auto& b) { return a.second > b.second; });

    for (size_t i = 0; i < std::min<size_t>(3, sorted_categories.size()); ++i)
        report->top3_categories.push_back(sorted_categories[i]);

    return report;
}

// Generates a report for a single day
std::vector<std::unique_ptr<Report>> FinanceManager::generate_daily_report(
    const std::chrono::system_clock::time_point& date)
{
    auto start = date;
    auto end = start + std::chrono::hours(24);

    std::vector<std::unique_ptr<Report>> result;
    result.emplace_back(create_report(start, end));
    return result;
}

// Generates a report for a 7-day week
std::vector<std::unique_ptr<Report>> FinanceManager::generate_weekly_report(
    const std::chrono::system_clock::time_point& week_start)
{
    auto start = week_start;
    auto end = start + std::chrono::hours(24 * 7);

    std::vector<std::unique_ptr<Report>> result;
    result.emplace_back(create_report(start, end));
    return result;
}

// Generates a report for a full calendar month
std::vector<std::unique_ptr<Report>> FinanceManager::generate_monthly_report(int y, int m)
{
    year_month_day start_ymd = year{ y } / month{ static_cast<unsigned>(m) } / day{ 1 };
    year_month_day end_ymd = start_ymd + months{ 1 };

    sys_days start = start_ymd;
    sys_days end = end_ymd;

    std::vector<std::unique_ptr<Report>> result;
    result.emplace_back(create_report(start, end));
    return result;
}


bool FinanceManager::save_reports(const std::vector<std::unique_ptr<Report>>& reports, const std::vector<ReportStorage::PeriodType>& types, const std::string& filename)
{
    return ReportStorage::save(reports, types, filename);
}

bool FinanceManager::load_reports(std::vector<std::unique_ptr<Report>>& reports, std::vector<ReportStorage::PeriodType>& types, const std::string& filename)
{
    return ReportStorage::load(reports, types, filename);
}