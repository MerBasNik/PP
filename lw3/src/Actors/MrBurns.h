//
// Created by Вадим Патрушев on 24.03.2026.
//

#ifndef ECONOMY_MRBURNS_H
#define ECONOMY_MRBURNS_H
#include "Actor.h"

#include <unordered_set>

class MrBurns : public Actor
{
public:
	MrBurns(Money salary);
	void DoAction(Bank& bank) override;
	void AddEmployee(Actor* employee);

private:
	Money m_salary;
	std::unordered_set<Actor*> m_employee;
};

#endif // ECONOMY_MRBURNS_H
