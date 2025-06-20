#pragma once
#include "BankCard.h"

class DebitCard : public BankCard {
public:
	using BankCard::BankCard;
	~DebitCard() override = default;

	const std::string& get_card_type() const override
	{
		return "Debit card";
	}
};
