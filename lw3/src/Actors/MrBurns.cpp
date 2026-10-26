//
// Created by Вадим Патрушев on 24.03.2026.
//

#include "MrBurns.h"

#include <format>

MrBurns::MrBurns(const Money salary)
	: Actor("MrBurns")
	, m_salary(salary)
{
}

void MrBurns::AddEmployee(Actor* employee)
{
	m_employees.push_back(employee);
}

void MrBurns::DoAction(Bank& bank)
{
	for (Actor* employee : m_employees)
	{
		const auto employeeAccountId = employee->GetAccountId();
		if (employeeAccountId.has_value() && PayFromAccount(bank, *employeeAccountId, m_salary))
		{
			const std::string message = std::format("paid salary {} to {}", m_salary, employee->GetName());
			Print(message);
		}
		else
		{
			const std::string message = std::format("skips turn: can't pay salary {} to {}", m_salary, employee->GetName());
			Print(message);
		}
	}
}
