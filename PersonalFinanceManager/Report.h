#pragma once
#include <chrono>
#include <map>
#include <vector>

// Holds summary data for a period
struct Report {
    std::chrono::system_clock::time_point period_start;
    std::chrono::system_clock::time_point period_end;

    double total_income = 0.0;  // Sum of incomes
    double total_expenses = 0.0;  // Sum of expenses

    std::map<int, double>            expenses_by_category; // category_id -> sum
    std::vector<std::pair<int, double>> top3_transactions;   // (transaction_id, amount)
    std::vector<std::pair<int, double>> top3_categories;     // (category_id, amount)
};