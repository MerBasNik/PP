//
// Created by Вадим Патрушев on 24.03.2026.
//

#ifndef ECONOMY_ECONOMY_H
#define ECONOMY_ECONOMY_H
#include "../Actors/Actor.h"
#include <vector>

class Economy
{
public:
	explicit Economy(int iterations);
	Economy(const Economy&) = delete;
	Economy& operator=(const Economy&) = delete;
	~Economy();

	void AddActor(Actor* actor);
	void SetBank(Bank* bank);

	void StartEconomy() const;
	void PrintResult() const;
	void VerifyConsistency() const;
	void CreateActors();

	[[nodiscard]] Bank* GetBank() const;
	[[nodiscard]] std::vector<Actor*>& GetActors();

private:
	const Money INITIAL_BALANCE = 9000;
	const int COUNT_ACTORS = 9;
	int m_iterations;
	Money m_initialBalance;
	Bank* m_bank;
	std::vector<Actor*> m_actors;

	void InitActorsAccount(const std::vector<Actor*>& actors, Money initMoney);
	[[nodiscard]] Money GetAllCashActorsMoney() const;
	[[nodiscard]] Money GetAllCashBankMoney() const;
	[[nodiscard]] Money GetAllAccountBalanceMoney() const;
	[[nodiscard]] Money GetAllMoney() const;
};

#endif // ECONOMY_ECONOMY_H
