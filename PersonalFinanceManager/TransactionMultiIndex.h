#pragma once
#include <chrono>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/hashed_index.hpp>		// Всё-таки подключил со второй попытки :D
#include <boost/multi_index/identity.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/mem_fun.hpp>

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

typedef bmi::multi_index_container
<
	Transaction,
	bmi::indexed_by
	<
		// by date, ordered, not unique
		bmi::ordered_non_unique
		<
			bmi::tag<ByDateSort>,
			bmi::const_mem_fun
			<
				Transaction,
				const std::chrono::system_clock::time_point&,
				&Transaction::get_timestamp
			>
		>,

		// by date, hash, not unique
		bmi::hashed_non_unique
		<
			bmi::tag<ByDateHashed>,
			bmi::const_mem_fun
			<
				Transaction,
				const std::chrono::system_clock::time_point&,
				&Transaction::get_timestamp
			>
		>,

		// by the amount of money, ordered, not unique
		bmi::ordered_non_unique
		<
			bmi::tag<ByAmountSort>,
			bmi::const_mem_fun
			<
				Transaction,
				double,
				&Transaction::get_amount
			>
		>,

		// by the amount of money, hash, not unique
		bmi::hashed_non_unique
		<
			bmi::tag<ByAmountHashed>,
			bmi::const_mem_fun
			<
				Transaction,
				double,
				&Transaction::get_amount
			>
		>,

		// by ID transactions, hash, unique
		bmi::hashed_unique
		<
			bmi::tag<ByTransactionID>,
			bmi::const_mem_fun
			<
				Transaction,
				int,
				&Transaction::get_transaction_id
			>
		>,

		// by ID accounts that owns a transaction, hash, unique
		bmi::hashed_unique
		<
			bmi::tag<ByAccountID>,
			bmi::const_mem_fun
			<
				Transaction,
				int,
				&Transaction::get_account_id
			>
		>
	>
>
TransactionMultiIndex;