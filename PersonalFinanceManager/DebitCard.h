#pragma once
#include "BankCard.h"

class DebitCard : public BankCard {
public:
	using BankCard::BankCard;
	~DebitCard() override = default;
};