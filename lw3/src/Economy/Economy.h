//
// Created by Вадим Патрушев on 24.03.2026.
//

#ifndef ECONOMY_ECONOMY_H
#define ECONOMY_ECONOMY_H
#include "src/Actors/Actor.h"
#include "src/Args/Args.h"
#include "src/Bank/Bank.h"
#include "src/Logger/Logger.h"
#include <atomic>
#include <chrono>
#include <memory>
#include <vector>

class Economy
{
public:
	static constexpr Money INITIAL_BALANCE = 10000;

	explicit Economy(bool quiet = true);
	Economy(const Economy&) = delete;
	Economy& operator=(const Economy&) = delete;
	void Run(Mode mode, std::chrono::milliseconds duration, const std::atomic<bool>& stopRequested);
	void PrintResult() const;
	void VerifyState() const;
	[[nodiscard]] unsigned long long GetRunOperationsCount() const;
	[[nodiscard]] std::chrono::duration<double> GetRunDuration() const;

private:
	Bank m_bank{ INITIAL_BALANCE };
	Logger m_logger;
	std::vector<std::unique_ptr<Actor>> m_actors;

	unsigned long long m_runOperationsCount = 0;
	std::chrono::duration<double> m_runDuration{};
	void CreateActors(bool quiet);
	[[nodiscard]] Money GetAllCashActorsMoney() const;
	[[nodiscard]] Money GetAllAccountBalanceMoney() const;
};

#endif // ECONOMY_ECONOMY_H
