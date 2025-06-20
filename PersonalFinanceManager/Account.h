#pragma once
#include <string>

// Provides a unified interface for Wallet and Card accounts
class Account {
protected:
	int id_;				// Unique ID account
	std::string name_;		// On an example Visa Gold or PayPal
	double balance_ = 0;	// Current balance

public:
	// Constructor and destructor
	Account() = default;
	Account(const int id, const std::string& name, const double initial_balance) : id_(id), name_(name), balance_(initial_balance){}
	virtual ~Account() = default;

	// Replenishment of account and money withdrawal
	virtual bool deposit(const double amount);
	virtual bool withdraw(const double amount);

	// Getters
	int get_id() const { return id_; }
	const std::string& get_owner_name() const { return name_; }
};