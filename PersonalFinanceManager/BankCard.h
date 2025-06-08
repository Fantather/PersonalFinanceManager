#pragma once
#include "Account.h"

// Собирает общие поля и методы для всех типов банковских карт (номер, имя, срок)
// Ни за что не отвечает сам по себе, потому что напрямую не создаётся (только через дочерние классы)
class BankCard : public Account {
protected:
	std::string card_number_;		// Номер карты
	std::string holder_name_;		// Имя владельца карты
	std::string expiration_date_;	// Срок действия (MM/YY)

public:
	BankCard(const int id, const std::string& name, const std::string& card_number, const std::string& holder_name, const std::string& expiration_date, const double initial_balance = default_initial_balance)
		: Account(id, name, initial_balance)
	{
		card_number_ = card_number;
		holder_name_ = holder_name;
		expiration_date_ = expiration_date;
	}

	virtual ~BankCard() = default;

	// Getters
	const std::string& getCardNumber() const { return card_number_; }
	const std::string& getHolderName() const { return holder_name_; }
	const std::string& getExpirationDate() const { return expiration_date_; }

	// Перегрузки deposit/withdraw по умолчанию можно перенести из Account.
};