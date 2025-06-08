#pragma once
#include <algorithm>

#include "BankCard.h"


// Поддерживает логику кредитной карты:

// Кредитный лимит(creditLimit),

// Текущая задолженность(currentDebt),

// Разрешает отрицательное состояние баланса, но лимитированно

//начисление процентов(метод accrueInterest()) — по желанию можно вызывать раз в месяц из FinanceManager или вручную.
class CreditCard : public BankCard {
protected:
	double credit_limit_;	// Максимальный кредитный лимит
	double current_debt_;	// Текущая задолженность
	double interest_rate_;	// Годовая процентная ставка

public:
	const static double default_current_debt;

	CreditCard(const int id, const std::string& name, const std::string& card_number, const std::string& holder_name, const std::string& expiration_date, const double credit_limit, const double interest_rate, const double initial_balance = default_initial_balance)
		: BankCard(id, name, card_number, holder_name, expiration_date, initial_balance)
	{
		credit_limit_ = credit_limit > 0.0 ? credit_limit : 0.0;
		current_debt_ = 0.0;
		interest_rate_ = interest_rate;
	}

	~CreditCard() override = default;

	
	void deposit(double amount) override;	// При пополнении гасим задолженность, если она есть, а всё что осталось увеличивает balance.
	bool withdraw(double amount) override;	// При списании (withdraw) можем «уйти в минус» до creditLimit. Если сумма превышает (creditLimit + текущий баланс), операция отклоняется

	// Getters
	double get_credit_limit() const { return credit_limit_; }
	double get_current_debt() const { return current_debt_; }
	double get_interest_rate() const { return interest_rate_; }

	// Метод, который начисляет проценты (например, вызывается раз в месяц)
	void accrue_interest();
};

const double CreditCard::default_current_debt = 0.0;