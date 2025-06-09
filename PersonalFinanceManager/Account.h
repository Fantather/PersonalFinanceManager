#pragma once
#include <string>

// Provides a unified interface for Wallet and Card accounts
class Account {
protected:
    int    id_;               // Unique account ID
    std::string name_;        // Account name (e.g. "Main Wallet", "Visa Gold")
    double balance_;          // Current balance

public:
    static const double default_initial_balance;  // Default starting balance

    // Constructor & Destructor
    Account(int id, const std::string& name, double initial_balance = default_initial_balance);
    virtual ~Account() = default;

    // Deposit / Withdraw interface
    virtual void deposit(double amount);
    virtual bool withdraw(double amount);

    // Getters
    int              get_id() const { return id_; }
    const std::string& get_name() const { return name_; }
    double           get_balance() const { return balance_; }
};