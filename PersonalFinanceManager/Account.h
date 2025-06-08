#pragma once
#include <string>

// Обеспечивает единый интерфейс для работы с кошельками (class Wallet) и картами (class Card)
class Account {
protected:
	int id_;				// Уникальный ID счёта
	std::string name_;		// Имя/название (например, "Основной кошелёк" или "Visa Gold")
	double balance_;		// Текущий баланс

	

public:
	// Default value for balance on card
	static const double default_initial_balance;

	// Constructor and Destructor
	Account(const int id, const std::string& name, double initial_balance = default_initial_balance);
	virtual ~Account() = default;

	// Единый интерфейс пополнения/списания
	virtual void deposit(double amount);	// Deposit card
	virtual bool withdraw(double amount);	// Withdraw money

	// Getter
	int get_id() const { return id_; }
	const std::string& get_name() const { return name_; }
	double get_balance() const { return balance_; }
};