#include "FinanceManager.h"

using namespace std::chrono;

/*--- Work with Cards ---*/
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

int FinanceManager::add_wallet(const std::string& name, double initial_balance) {
    int id = next_account_id_++;
    accounts_[id] = std::make_unique<Wallet>(id, name, initial_balance);
    return id;
}

bool FinanceManager::remove_account(int account_id) {
    return accounts_.erase(account_id) > 0;
}

Account* FinanceManager::get_account(int account_id) {
    auto it = accounts_.find(account_id);
    return it != accounts_.end() ? it->second.get() : nullptr;
}


/*--- Work with Categories ---*/
int FinanceManager::add_category(const std::string& name, const int parentId) {
    int id = next_category_id_++;
    category_[id] = std::make_unique<Category>(name, id, parentId);
    return id;
}

bool FinanceManager::remove_category(const int category_id) {
    return category_.erase(category_id) > 0;
}

Category* FinanceManager::get_category(const int category_id) {
    auto it = category_.find(category_id);
    return it != category_.end() ? it->second.get() : nullptr;
}


bool FinanceManager::add_income(const int account_id, const double amount, const std::chrono::system_clock::time_point& timestamp) {
    Account* acc = get_account(account_id);
    if (!acc || amount <= 0) return false;      // Check the parameters for validity
    acc->deposit(amount);                       // Income

    int id = next_transaction_id_++;

    transactions_.insert(Income(amount, id, account_id, timestamp));                // Add the transaction to Boost
    return true;
}

bool FinanceManager::add_expense(const int account_id, const double amount, const int category_id, const std::chrono::system_clock::time_point& timestamp) {
    Account* acc = get_account(account_id);
    if (!acc || amount <= 0) return false;      // Check the parameters for validity
    if (!acc->withdraw(amount)) return false;   // Ttrying to withdraw money

    int id = next_transaction_id_++;

    transactions_.insert(Expence(amount, id, account_id, timestamp, category_id));  // Add the transaction to Boost
    return true;
}


/*--- Work with Reports ---*/
std::vector<const Transaction*> FinanceManager::create_report(const std::chrono::system_clock::time_point& start, const std::chrono::system_clock::time_point& end)
{
    const auto& idx = transactions_.get<ByDateSort>(); // Упорядоченный по дате
    std::vector<const Transaction*> result;
    auto it = idx.lower_bound(start);
    while (it != idx.end() && it->get_timestamp() < end) {
        if (it->is_expence()) result.push_back(&*it);
        ++it;
    }
    return result;
}

std::unique_ptr<Report> FinanceManager::build_report(const std::chrono::system_clock::time_point& start, const std::chrono::system_clock::time_point& end)
{
    auto report = std::make_unique<Report>(start, end);

    // Categories expenses
    std::map<int, double> by_category;

    for (const auto& tx : transactions_.get<ByDateSort>())
    {
        if (!tx.is_expence())
            continue;

        auto timestamp = tx.get_timestamp();
        if (timestamp < start || timestamp >= end)
            continue;

        int category_id = static_cast<const Expence&>(tx).get_category_id();
        by_category[category_id] += tx.get_amount();

        report->total_expence_ += tx.get_amount();
    }
    report->expenses_by_category = by_category;

    // TOP-3 transactions by sum using the index ByAmountSort
    std::vector<std::pair<int, double>> top3_txs;

    const auto& amount_subtable = transactions_.get<ByAmountSort>();
    auto it_amount = amount_subtable.end();

    while (it_amount != amount_subtable.begin() && top3_txs.size() < 3) {
        --it_amount;
        if (!it_amount->is_expence())
            continue;
        auto ts = it_amount->get_timestamp();

        if (ts < start || ts >= end)
            continue;

        top3_txs.emplace_back(it_amount->get_transaction_id(), it_amount->get_amount());
    }
    report->top3_transactions = top3_txs;

    // TOP-3 categories in terms of expenses
    std::vector<std::pair<int, double>> category_amounts(by_category.begin(), by_category.end());
    std::sort(category_amounts.begin(), category_amounts.end(), [](auto& a, auto& b) { return a.second > b.second; });

    if (category_amounts.size() > 3)
    {
        category_amounts.resize(3);
    }
    report->top3_categories = category_amounts;

    return report;
}


std::vector<std::unique_ptr<Report>> FinanceManager::generate_daily_report(
    const system_clock::time_point& date)
{
    auto day_start = floor<days>(date);
    auto day_end = day_start + days{ 1 };
    return { build_report(day_start, day_end) };
}

std::vector<std::unique_ptr<Report>> FinanceManager::generate_weekly_report(
    const system_clock::time_point& week_start)
{
    auto start = floor<days>(week_start);
    auto end = start + days{ 7 };
    return { build_report(start, end) };
}

std::vector<std::unique_ptr<Report>> FinanceManager::generate_monthly_report(
    int my_year, int my_month)
{
    year_month_day ymd{ year{my_year}, month{ static_cast<unsigned>(my_month) }, day{1} };
    auto start = sys_days{ ymd };
    auto end = start + months{ 1 };
    return { build_report(start, end) };
}
