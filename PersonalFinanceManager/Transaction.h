#pragma once
#include <string>
#include <chrono>

// Определяет каркас любой транзакции: время, сумму, «откуда / куда».
// Класс абстрактный: конкретный смысл имеет либо Income(пополнение), либо Expense(расход)

class Transaction {
protected:
	std::chrono::system_clock::time_point timestamp_;	// Дата/время операции
	double amount_;										// Сумма (для дохода положительная, для расхода можно тоже положительная, но тип «Expense»)
	int id_;											// Уникальный ID транзакции
	int account_id_;									// ID счёта (Account), из/в которого проводим деньги

public:
	Transaction(int account_id, double amount, const std::chrono::system_clock::time_point& timestamp, int id)
		: account_id_(account_id), amount_(amount), timestamp_(timestamp), id_(id) {}
	virtual ~Transaction() = default;

	int     get_id() const { return id_; }
	auto    get_timestamp() const { return timestamp_; }  // возвращает time_point
	double  get_amount() const { return amount_; }
	int     get_account_id() const { return account_id_; }

	// Чисто виртуальный метод, чтобы отличать расход от дохода
	virtual bool is_expense() const = 0;
};