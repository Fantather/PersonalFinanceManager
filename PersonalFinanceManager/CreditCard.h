#pragma once
#include <algorithm>
#include "BankCard.h"

// Credit card with limit, debt, interest
class CreditCard : public BankCard {
private:
	double credit_limit_;		// Maximum credit
	double current_debt_;		// Outstanding debt
	double interest_rate_;		// Annual interest rate in decimal format (e.g., 0.15 for 15% per year)
public:
	// Constructor and Destructor
	CreditCard(
		const int id, const std::string& name, const std::string& owner_name, const std::string& card_number, const std::string& expiration_date,
		const double credit_limit, const double interest_rate, const double initial_balance = 0.0, const double current_debt = 0.0)
		: BankCard(id, name, owner_name, card_number, expiration_date, initial_balance), credit_limit_(credit_limit), current_debt_(current_debt), interest_rate_(interest_rate) {}
	~CreditCard() override = default;

	// Override deposit / withdraw
	bool deposit(double amount);
	bool withdraw(const double amount);

	// Getters
	double get_credit_limit() const { return credit_limit_; }
	double get_current_debt() const { return current_debt_; }
	double get_interest_rate() const { return interest_rate_; }
	const std::string& get_card_type() const override { return "Debit card"; }

	// Set current debt (Used in loading)
	void set_current_debt(const double debt) { current_debt_ = debt; }

	// Accrue monthly interest on outstanding debt
	void accrue_interest();
};