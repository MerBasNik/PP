//
// Created by Вадим Патрушев on 24.03.2026.
//

#include "Economy.h"

#include "src/Actors/Apu.h"
#include "src/Actors/Bart.h"
#include "src/Actors/Chester.h"
#include "src/Actors/Homer.h"
#include "src/Actors/Lisa.h"
#include "src/Actors/Marge.h"
#include "src/Actors/MrBurns.h"
#include "src/Actors/Nelson.h"
#include "src/Actors/Smithers.h"
#include "src/Bank/BankOperationError.h"
#include <iostream>

Economy::Economy(const int iterations)
	: m_iterations(iterations)
	, m_bank(new Bank(INITIAL_BALANCE))
{
	if (iterations < 0)
	{
		throw std::invalid_argument("iterations must be greater than zero");
	}
	m_iterations = iterations;
	m_initialBalance = m_bank->GetCash();
	CreateActors();
}

Economy::~Economy()
{
	for (const auto& actor : m_actors)
	{
		delete actor;
	}
}

void Economy::AddActor(Actor* actor)
{
	m_actors.emplace_back(actor);
}

std::vector<Actor*>& Economy::GetActors()
{
	return m_actors;
}

Bank* Economy::GetBank() const
{
	return m_bank;
}

void Economy::SetBank(Bank* bank)
{
	m_bank = bank;
}

void Economy::CreateActors()
{
	const Money initMoney = INITIAL_BALANCE / COUNT_ACTORS;
	std::vector<Actor*> actors;

	auto* mrBurns = new MrBurns(1500);
	actors.push_back(mrBurns);
	auto* apu = new Apu(200);
	actors.push_back(apu);
	auto* marge = new Marge(600);
	actors.push_back(marge);
	auto* homer = new Homer(500, 200, 100);
	actors.push_back(homer);
	auto* bart = new Bart(initMoney, 300);
	AddActor(bart);
	auto* lisa = new Lisa(initMoney, 300);
	AddActor(lisa);
	auto* chester = new Chester(1000, 700);
	actors.push_back(chester);
	auto* nelson = new Nelson(200);
	AddActor(nelson);
	auto* smithers = new Smithers(500);
	actors.push_back(smithers);

	InitActorsAccount(actors, initMoney);

	chester->SetApu(apu);
	chester->SetHomer(homer);
	nelson->SetApu(apu);
	nelson->SetBart(bart);
	smithers->SetApu(apu);
	mrBurns->AddEmployee(homer);
	apu->SetMrBurns(mrBurns);
	bart->SetApu(apu);
	lisa->SetApu(apu);
	marge->SetApu(apu);
	homer->SetMrBurns(mrBurns);
	homer->SetBart(bart);
	homer->SetLisa(lisa);
	homer->SetMarge(marge);
}

void Economy::InitActorsAccount(const std::vector<Actor*>& actors, const Money initMoney)
{
	for (const auto& actor : actors)
	{
		actor->SetAccountId(m_bank->OpenAccount());
		AddActor(actor);
		m_bank->DepositMoney(actor->GetAccountId().value(), initMoney);
	}
}

void Economy::StartEconomy() const
{
	for (int i = 0; i < m_iterations; ++i)
	{
		std::cout << i + 1 << " iteration" << std::endl;
		for (const auto& actor : m_actors)
		{
			try
			{
				actor->DoAction(*m_bank);
			}
			catch (const BankOperationError& e)
			{
				std::cout << "ERROR: " << e.what() << std::endl;
			}
		}
		std::cout << std::endl;
	}
	PrintResult();
}

void Economy::PrintResult() const
{
	const Money allAccountBalanceMoney = GetAllAccountBalanceMoney();
	const Money allCashMoney = GetAllCashActorsMoney();
	std::cout << "All account balances: " << allAccountBalanceMoney << std::endl;
	std::cout << "All cash money: " << allCashMoney << std::endl;
	try
	{
		VerifyConsistency();
		std::cout << "Bank system is in consistent state" << std::endl;
	}
	catch (const BankOperationError& e)
	{
		std::cout << "ERROR: " << e.what() << std::endl;
	}
}

Money Economy::GetAllCashBankMoney() const
{
	const Money allBankMoney = m_bank->GetCash();
	return allBankMoney;
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
			allAccountBalanceMoney += m_bank->GetAccountBalance(*accountId);
		}
	}
	return allAccountBalanceMoney;
}

Money Economy::GetAllMoney() const
{
	const Money allCashMoney = GetAllCashBankMoney();
	const Money allAccountBalanceMoney = GetAllAccountBalanceMoney();
	const Money allMoney = allCashMoney + allAccountBalanceMoney;
	return allMoney;
}

void Economy::VerifyConsistency() const
{
	const Money allCashBankMoney = GetAllCashBankMoney();
	const Money allCashActorsMoney = GetAllCashActorsMoney();
	if (allCashBankMoney != allCashActorsMoney)
	{
		throw BankOperationError("Different cash bank money");
	}
	const Money allMoney = GetAllMoney();
	if (allMoney != INITIAL_BALANCE)
	{
		throw BankOperationError("Different balance all money and initial");
	}
}
