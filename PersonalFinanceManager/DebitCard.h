#pragma once
#include "BankCard.h"

// Debit card: uses base card behavior
class DebitCard : public BankCard {
public:
    using BankCard::BankCard;
    ~DebitCard() override = default;
};
