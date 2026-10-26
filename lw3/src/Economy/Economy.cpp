//
// Created by Вадим Патрушев on 24.03.2026.
//

#include "Economy.h"

#include "src/Actors/Apu.h"
#include "src/Actors/Customer.h"
#include "src/Actors/Homer.h"
#include "src/Actors/MrBurns.h"
#include "src/Bank/BankOperationError.h"
#include <iostream>
#include <thread>

constexpr Money BURNS_SALARY = 1500;
constexpr Money HOMER_MONEY_FOR_MARGE = 500;
constexpr Money HOMER_ELECTRICITY_BILL = 200;
constexpr Money HOMER_MONEY_FOR_CHILDREN = 200;
constexpr Money MARGE_PURCHASE = 500;
constexpr Money CHILD_PURCHASE = 100;
constexpr Money APU_ELECTRICITY_BILL = 700;
constexpr Money BURNS_INITIAL_BALANCE = 5000;
constexpr Money HOMER_INITIAL_BALANCE = 1000;
constexpr Money APU_INITIAL_BALANCE = 1000;
constexpr Money HOMER_INITIAL_CASH = 1000;
constexpr Money MARGE_INITIAL_CASH = 1000;
constexpr Money CHILD_INITIAL_CASH = 500;

bool Economy::ShouldStop(const std::atomic<bool>& isStop, const Clock::time_point end)
{
	return isStop || Clock::now() >= end;
}

Economy::Economy(const bool quiet)
{
	CreateActors(quiet);
}

void Economy::CreateActors(const bool quiet)
{
	auto mrBurns = std::make_unique<MrBurns>(BURNS_SALARY);
	auto homer = std::make_unique<Homer>(HOMER_MONEY_FOR_MARGE, HOMER_ELECTRICITY_BILL, HOMER_MONEY_FOR_CHILDREN);
	auto marge = std::make_unique<Customer>("Marge", MARGE_PURCHASE);
	auto bart = std::make_unique<Customer>("Bart", CHILD_PURCHASE);
	auto lisa = std::make_unique<Customer>("Lisa", CHILD_PURCHASE);
	auto apu = std::make_unique<Apu>(APU_ELECTRICITY_BILL);

	mrBurns->SetAccountId(m_bank.OpenAccount());
	homer->SetAccountId(m_bank.OpenAccount());
	apu->SetAccountId(m_bank.OpenAccount());

	m_bank.DepositMoney(*mrBurns->GetAccountId(), BURNS_INITIAL_BALANCE);
	m_bank.DepositMoney(*homer->GetAccountId(), HOMER_INITIAL_BALANCE);
	m_bank.DepositMoney(*apu->GetAccountId(), APU_INITIAL_BALANCE);
	homer->GetWallet().Add(HOMER_INITIAL_CASH);
	marge->GetWallet().Add(MARGE_INITIAL_CASH);
	bart->GetWallet().Add(CHILD_INITIAL_CASH);
	lisa->GetWallet().Add(CHILD_INITIAL_CASH);

	mrBurns->AddEmployee(homer.get());
	homer->SetMrBurns(mrBurns.get());
	homer->SetMarge(marge.get());
	homer->SetBart(bart.get());
	homer->SetLisa(lisa.get());
	marge->SetApu(apu.get());
	bart->SetApu(apu.get());
	lisa->SetApu(apu.get());
	apu->SetMrBurns(mrBurns.get());

	m_actors.push_back(std::move(mrBurns));
	m_actors.push_back(std::move(homer));
	m_actors.push_back(std::move(marge));
	m_actors.push_back(std::move(bart));
	m_actors.push_back(std::move(lisa));
	m_actors.push_back(std::move(apu));

	for (const auto& actor : m_actors)
	{
		actor->SetQuiet(quiet);
	}
}

void Economy::Run(const Mode mode, const std::chrono::milliseconds duration, const std::atomic<bool>& isStop)
{
	const auto operationsBefore = m_bank.GetOperationsCount();
	const auto start = Clock::now();
	const auto end = start + duration;

	mode == Mode::Single ? SingleRun(isStop, end) : ParallelRun(isStop, end);

	m_runDuration = Clock::now() - start;
	m_runOperationsCount = m_bank.GetOperationsCount() - operationsBefore;
}

void Economy::ParallelRun(const std::atomic<bool>& isStop, Clock::time_point end)
{
	std::vector<std::jthread> threads;
	threads.reserve(m_actors.size());
	for (const auto& actor : m_actors)
	{
		threads.emplace_back([this, &actor, &isStop, end] {
			while (!ShouldStop(isStop, end))
			{
				actor->DoAction(m_bank);
			}
		});
	}
}

void Economy::SingleRun(const std::atomic<bool>& isStop, Clock::time_point end)
{
	while (!ShouldStop(isStop, end))
	{
		for (const auto& actor : m_actors)
		{
			if (ShouldStop(isStop, end))
			{
				break;
			}
			actor->DoAction(m_bank);
		}
	}
}

void Economy::PrintResult() const
{
	const double seconds = m_runDuration.count();
	const Money bankCash = m_bank.GetCash();
	const auto operationsCount = m_bank.GetOperationsCount();
	const Money actorsCash = GetAllCashActorsMoney();
	const Money accountsMoney = GetAllAccountBalanceMoney();

	std::cout << "Duration: " << seconds << std::endl;
	std::cout << "Operations: " << operationsCount << std::endl;
	std::cout << "Bank cash: " << bankCash << ", all actors cash: " << actorsCash << std::endl;
	std::cout << "Bank cash + all account balances: " << bankCash + accountsMoney
		<< ", initial balance: " << INITIAL_BALANCE << std::endl;
}

void Economy::VerifyState() const
{
	const Money bankCash = m_bank.GetCash();
	if (bankCash != GetAllCashActorsMoney())
	{
		throw BankOperationError("bank cash differs from cash in actors' wallets");
	}
	if (bankCash + GetAllAccountBalanceMoney() != INITIAL_BALANCE)
	{
		throw BankOperationError("total money differs from initial balance");
	}
}

unsigned long long Economy::GetRunOperationsCount() const
{
	return m_runOperationsCount;
}

std::chrono::duration<double> Economy::GetRunDuration() const
{
	return m_runDuration;
}

Money Economy::GetAllCashActorsMoney() const
{
	Money allCashActorsMoney = 0;
	for (const auto& actor : m_actors)
	{
		allCashActorsMoney += actor->GetCash();
	}
	return allCashActorsMoney;
}

Money Economy::GetAllAccountBalanceMoney() const
{
	Money allAccountBalanceMoney = 0;
	for (const auto& actor : m_actors)
	{
		const auto accountId = actor->GetAccountId();
		if (accountId.has_value())
		{
			allAccountBalanceMoney += m_bank.GetAccountBalance(*accountId);
		}
	}
	return allAccountBalanceMoney;
}
