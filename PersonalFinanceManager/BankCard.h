#pragma once
#include <string>
#include "Account.h"

// Base for all card types (number, holder, expiry)
class BankCard : public Account {
protected:
    std::string card_number_;     // Card number
    std::string holder_name_;     // Cardholder name
    std::string expiration_date_; // Expiry (MM/YY)

public:
    BankCard(int id,
        const std::string& name,
        const std::string& card_number,
        const std::string& holder_name,
        const std::string& expiration_date,
        double initial_balance = default_initial_balance)
        
        : Account(id, name, initial_balance),
        card_number_(card_number),
        holder_name_(holder_name),
        expiration_date_(expiration_date) {}

    virtual ~BankCard() = default;

    // Getters
    const std::string& get_card_number() const { return card_number_; }
    const std::string& get_holder_name() const { return holder_name_; }
    const std::string& get_expiration_date() const { return expiration_date_; }
};