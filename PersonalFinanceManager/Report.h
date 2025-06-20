#pragma once
#include <chrono>
#include <map>
#include <vector>

#include "Category.h"

// Holds summary data for a period
struct Report {
	std::chrono::system_clock::time_point period_start_;
	std::chrono::system_clock::time_point period_end_;

	Report(const std::chrono::system_clock::time_point& period_start, const std::chrono::system_clock::time_point& period_end)
		:period_start_(period_start), period_end_(period_end) {}

	double total_expence_ = 0.0;

	std::map<int, double> expenses_by_category;				 // (category_id, sum)
	std::vector<std::pair<int, double>> top3_transactions;   // (transaction_id, amount)
	std::vector<std::pair<int, double>> top3_categories;     // (category_id, amount)
};