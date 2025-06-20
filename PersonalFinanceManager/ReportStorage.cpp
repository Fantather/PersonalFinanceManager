#include "ReportStorage.h"
#include "Report.h"

#include <fstream>
#include <format>      // for std::format
#include <chrono>      // for std::chrono types
#include <string>


// Helper: format a time_point into string
static std::string format_timestamp(const std::chrono::system_clock::time_point& tp) {
    return std::format("{:%Y-%m-%d %H:%M:%S}", tp);
}

// save_reports:
bool ReportStorage::save_reports(const std::vector<std::unique_ptr<Report>>& reports, const std::string& filename)
{
    std::ofstream out(filename, std::ios::app);
    if (!out.is_open()) {
        return false;
    }

    for (const auto& r : reports) {
        // --- Write report header with start/end timestamps ---
        out << "=== Report from "
            << format_timestamp(r->period_start_)
            << " to "
            << format_timestamp(r->period_end_)
            << " ===\n";

        // --- Write total expense ---
        out << "Total expense: "
            << r->total_expence_
            << "\n\n";

        // --- Write expense breakdown by category ---
        out << "Expenses by category (category_id: amount):\n";
        for (const auto& [cat_id, sum] : r->expenses_by_category) {
            out << "  [" << cat_id << "]: " << sum << "\n";
        }
        out << "\n";

        // --- Write top-3 individual transactions ---
        out << "Top-3 transactions (transaction_id: amount):\n";
        for (const auto& [tx_id, amt] : r->top3_transactions) {
            out << "  [" << tx_id << "]: " << amt << "\n";
        }
        out << "\n";

        // --- Write top-3 categories by total expense ---
        out << "Top-3 categories (category_id: amount):\n";
        for (const auto& [cat_id, sum] : r->top3_categories) {
            out << "  [" << cat_id << "]: " << sum << "\n";
        }
        out << "\n\n";
    }

    out.close();
    return true;
}