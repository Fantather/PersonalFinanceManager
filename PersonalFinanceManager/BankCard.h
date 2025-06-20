#pragma once
#include <string>
#include "Account.h"

// Collects common fields and methods for all types of bank cards (number, name, term)
// He does not answer by itself, because it is not directly created (only through subsidiaries)
class BankCard : public Account {
protected:
	std::string owner_name_;
	std::string card_number_;
	std::string expiration_date_;

public:
	// Constructor and destructor
	BankCard(const int id, const std::string& name, const std::string& owner_name, const std::string& card_number, const std::string& expiration_date, const double initial_balance = 0.0)
		: Account(id, name, initial_balance), owner_name_(owner_name), card_number_(card_number), expiration_date_(expiration_date) {}
	virtual ~BankCard() = default;

	// Getters
	const std::string& get_owner_name() const { return owner_name_; }
	const std::string& get_card_number() const { return card_number_; }
	const std::string& get_expiration_date() const { return expiration_date_; }
	virtual const std::string& get_card_type() const = 0;		// To make a class abstract

	// withdraw/deposit work by default
};