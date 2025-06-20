#pragma once
#include "Account.h"

// Ordinary wallet (cash or virtual account)
class Wallet : public Account {
public:
	using Account::Account;
	~Wallet() override = default;

	// For the wallet withdraw/deposit work by default
};