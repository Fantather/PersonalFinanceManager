#include <iostream>
#include <chrono>
#include <thread>

#include "FinanceManager.h"
#include "ReportStorage.h"

int main() {
    using namespace std::chrono;

    // Create manager
    FinanceManager manager;

    // Add accounts
    int walletId = manager.add_wallet("MyWallet", 500.0);

    int debitCardId = manager.add_debitCard(
        "Visa Gold",                          // name
        "12345",                              // card_number (тип string!)
        "Alice",                              // holder name
        "12/26",                              // expiration date
        1000.0                                // initial balance
    );

    int creditCardId = manager.add_creditCard(
        "MasterCard Platinum",               // name
        "9876-5432-1098-7654",               // card_number
        "Bob",                               // holder name
        "11/25",                             // expiration date
        2000.0,                              // credit limit
        0.18,                                // interest rate
        500.0                                // initial balance
    );

    // Add categories
    int foodCat = manager.add_category("Food");
    int transportCat = manager.add_category("Transport");
    int entertainmentCat = manager.add_category("Entertainment");

    // Record some transactions
    auto t0 = system_clock::now();
    manager.add_income(walletId, 200.0, t0 - hours(24));                // yesterday income
    manager.add_expense(walletId, 50.0, foodCat, t0 - hours(23));
    manager.add_expense(debitCardId, 100.0, transportCat, t0 - hours(2));
    manager.add_expense(creditCardId, 150.0, entertainmentCat, t0 - minutes(30));
    manager.add_income(debitCardId, 300.0, t0 - minutes(15));

    // Generate reports for today
    auto today = floor<days>(t0);
    auto dailyReports = manager.generate_daily_report(today + hours(12)); // midday to include all
    auto weeklyReports = manager.generate_weekly_report(today);
    auto monthlyReports = manager.generate_monthly_report(static_cast<int>(year_month_day{ year{2025}, month{6}, day{1} }.year().operator int()),
        static_cast<unsigned>(year_month_day{ year{2025}, month{6}, day{1} }.month()));

    // Save reports to file
    const std::string filename = "reports.txt";
    if (ReportStorage::save_reports(dailyReports, filename)) {
        std::cout << "Daily reports saved to " << filename << std::endl;
    }
    if (ReportStorage::save_reports(weeklyReports, filename)) {
        std::cout << "Weekly reports appended to " << filename << std::endl;
    }
    if (ReportStorage::save_reports(monthlyReports, filename)) {
        std::cout << "Monthly reports appended to " << filename << std::endl;
    }

    // Optionally, print a summary to console
    for (const auto& reportPtr : dailyReports) {
        const Report& r = *reportPtr;
        std::cout << "--- Report Summary (Daily) ---" << std::endl;
        std::cout << "Total expense: " << r.total_expence_ << std::endl;
        std::cout << "Expenses by category:" << std::endl;
        for (auto& [cid, sum] : r.expenses_by_category) {
            std::cout << "  Category id " << cid << ": " << sum << std::endl;
        }
        std::cout << "Top 3 transactions:" << std::endl;
        for (auto& [txid, amt] : r.top3_transactions) {
            std::cout << "  Transaction id " << txid << ": " << amt << std::endl;
        }
        std::cout << "Top 3 categories:" << std::endl;
        for (auto& [cid, amt] : r.top3_categories) {
            std::cout << "  Category id " << cid << ": " << amt << std::endl;
        }
    }

    return 0;
}



// Третье задание
// Создайте систему управления персональными финансами.
// Система должна иметь следующие возможности :

// Наличие разных кошельков и карт(дебитных / кредитных).

// Пополнение кошельков и карт.

// Внесение затрат.
// Каждая затрата относится к определенной категории.

// Формирование отчётов по затратам и категориям :
// день;
// неделя;
// месяц.

// Формирование рейтингов по максимальным суммам :
// топ - 3 затрат :
// неделя;
// месяц

// топ - 3 категорий:
// неделя;
// месяц.

//Сохранение отчётов и рейтингов в файл.