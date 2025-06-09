#include "FinanceManager.h"
#include <iostream>
#include <chrono>
#include <iomanip>

using namespace std;
using namespace std::chrono;

void print_report(const Report& r) {
    auto start_c = system_clock::to_time_t(r.period_start);
    auto end_c = system_clock::to_time_t(r.period_end);

    std::tm tm_start{}, tm_end{};
    localtime_s(&tm_start, &start_c);
    localtime_s(&tm_end, &end_c);

    std::cout << "Report from " << std::put_time(&tm_start, "%F") << " to " << std::put_time(&tm_end, "%F") << ":\n";
    cout << "Total Income: " << r.total_income << "\n";
    cout << "Total Expenses: " << r.total_expenses << "\n";

    cout << "Expenses by Category:\n";
    for (const auto& [cat, sum] : r.expenses_by_category) {
        cout << "  Category " << cat << ": " << sum << "\n";
    }

    cout << "Top 3 Transactions:\n";
    for (const auto& [id, amount] : r.top3_transactions) {
        cout << "  TX" << id << ": " << amount << "\n";
    }

    cout << "Top 3 Categories:\n";
    for (const auto& [cat, amount] : r.top3_categories) {
        cout << "  Category " << cat << ": " << amount << "\n";
    }
    cout << "----------------------------\n";
}

int main() {
    FinanceManager fm;

    // Создаем аккаунты
    int wallet_id = fm.add_wallet("Main Wallet", 5000);
    int food_cat_id = fm.add_category("Food");
    int transport_cat_id = fm.add_category("Transport");

    // Добавляем транзакции
    auto now = system_clock::now();
    fm.add_income(wallet_id, 30000, now - hours(48));  // 2 дня назад
    fm.add_expense(wallet_id, 1500, food_cat_id, now - hours(24));  // вчера
    fm.add_expense(wallet_id, 500, transport_cat_id, now - hours(12));  // сегодня
    fm.add_expense(wallet_id, 2500, food_cat_id, now);  // сейчас

    // Генерируем отчеты
    auto daily_reports = fm.generate_daily_report(now);
    auto weekly_reports = fm.generate_weekly_report(now - days(7));

    // Сохраняем отчеты
    vector<ReportStorage::PeriodType> types = {
        ReportStorage::Daily,
        ReportStorage::Weekly
    };

    vector<unique_ptr<Report>> all_reports;
    all_reports.push_back(move(daily_reports[0]));
    all_reports.push_back(move(weekly_reports[0]));

    fm.save_reports(all_reports, types, "reports.bin");

    // Загружаем и выводим отчеты
    vector<unique_ptr<Report>> loaded_reports;
    vector<ReportStorage::PeriodType> loaded_types;

    if (fm.load_reports(loaded_reports, loaded_types, "reports.bin")) {
        cout << "Loaded reports:\n";
        for (size_t i = 0; i < loaded_reports.size(); ++i) {
            cout << "Type: ";
            switch (loaded_types[i]) {
            case ReportStorage::Daily: cout << "Daily\n"; break;
            case ReportStorage::Weekly: cout << "Weekly\n"; break;
            case ReportStorage::Monthly: cout << "Monthly\n"; break;
            }
            print_report(*loaded_reports[i]);
        }
    }
    else {
        cerr << "Failed to load reports!\n";
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