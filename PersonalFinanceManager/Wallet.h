#pragma once
#include "Account.h"

// Обычный «кошелёк»(наличные или виртуальный счёт внутри приложения), без дополнительных ограничений или процентов.

class Wallet : public Account {
public:
	Wallet(int id, const std::string& name, double initial_balance = default_initial_balance) : Account(id, name, initial_balance) {}
	~Wallet() override = default;

	// Для кошелька withdraw/deposit работают «по умолчанию» (с учётом достаточности средств)
};