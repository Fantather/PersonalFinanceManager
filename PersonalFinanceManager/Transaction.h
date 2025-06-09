#pragma once
#include <chrono>

// Abstract base for all transactions: timestamp, amount, account
class Transaction {
protected:
    int                                          account_id_;
    double                                       amount_;
    std::chrono::system_clock::time_point        timestamp_;
    int                                          id_;

public:
    Transaction(int account_id,
        double amount,
        const std::chrono::system_clock::time_point& timestamp,
        int id)
        : account_id_(account_id),
        amount_(amount),
        timestamp_(timestamp),
        id_(id)
    {}

    virtual ~Transaction() = default;

    // Pure virtual to distinguish Income vs Expense
    virtual bool is_expense() const = 0;

    // Getters
    int                                      get_id() const { return id_; }
    int                                      get_account_id() const { return account_id_; }
    double                                   get_amount() const { return amount_; }
    std::chrono::system_clock::time_point    get_timestamp() const { return timestamp_; }
};