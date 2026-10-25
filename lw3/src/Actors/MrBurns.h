//
// Created by Вадим Патрушев on 24.03.2026.
//

#ifndef ECONOMY_MRBURNS_H
#define ECONOMY_MRBURNS_H
#include "Actor.h"
#include <vector>

class MrBurns : public Actor
{
public:
	explicit MrBurns(Money salary);
	void DoAction(Bank& bank) override;
	void AddEmployee(Actor* employee);

private:
	Money m_salary;
	std::vector<Actor*> m_employees;
};

#endif // ECONOMY_MRBURNS_H
