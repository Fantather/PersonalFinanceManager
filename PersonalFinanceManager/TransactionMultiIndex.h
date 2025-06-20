#pragma once
#include <chrono>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/hashed_index.hpp>		// Всё-таки подключил со второй попытки :D
#include <boost/multi_index/identity.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/mem_fun.hpp>
#include<boost/multi_index/global_fun.hpp>

#include "Transaction.h"


namespace bmi = boost::multi_index;
// namespace time_ = std::chrono::system_clock::time_point;

// Tags for Boost
struct ByDateSort {};
struct ByDateHashed {};
struct ByAmountSort {};
struct ByAmountHashed {};
struct ByTransactionID {};
struct ByAccountID {};

// Extractors
inline const std::chrono::system_clock::time_point& timepoint_extract(std::shared_ptr<Transaction> trx)
{
	return trx->get_timestamp();
}

inline double amount_extract(std::shared_ptr<Transaction> trx)
{
	return trx->get_amount();
}

inline int transaction_id_extract(std::shared_ptr<Transaction> trx)
{
	return trx->get_transaction_id();
}

inline int account_id_extract(std::shared_ptr<Transaction> trx)
{
	return trx->get_account_id();
}


typedef bmi::multi_index_container
<
	std::shared_ptr<Transaction>,
	bmi::indexed_by
	<
		// by date, ordered, not unique
		bmi::ordered_non_unique
		<
			bmi::tag<ByDateSort>,
			bmi::global_fun
			<
				std::shared_ptr<Transaction>,
				const std::chrono::system_clock::time_point&,
				&timepoint_extract
			>
		>,

		// by date, hash, not unique
		/*bmi::hashed_non_unique
		<
			bmi::tag<ByDateHashed>,
			bmi::const_mem_fun
			<
				Transaction,
				const std::chrono::system_clock::time_point&,
				&Transaction::get_timestamp
			>
		>,*/

		// by the amount of money, ordered, not unique
		bmi::ordered_non_unique
		<
			bmi::tag<ByAmountSort>,
			bmi::global_fun
			<
				std::shared_ptr<Transaction>,
				double,
				&amount_extract
			>
		>,

		// by the amount of money, hash, not unique
		bmi::hashed_non_unique
		<
			bmi::tag<ByAmountHashed>,
			bmi::global_fun
			<
				std::shared_ptr<Transaction>,
				double,
				&amount_extract
			>
		>,

		// by ID transactions, hash, unique
		bmi::hashed_unique
		<
			bmi::tag<ByTransactionID>,
			bmi::global_fun
			<
				std::shared_ptr<Transaction>,
				int,
				&transaction_id_extract
			>
		>,

		// by ID accounts that owns a transaction, hash, unique
		bmi::hashed_unique
		<
			bmi::tag<ByAccountID>,
			bmi::global_fun
			<
				std::shared_ptr<Transaction>,
				int,
				&account_id_extract
			>
		>
	>
>
TransactionMultiIndex;