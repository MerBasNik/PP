//
// Created by Вадим Патрушев on 24.03.2026.
//

#include "MrBurns.h"
#include <iostream>

MrBurns::MrBurns(const Money salary)
	: Actor("MrBurns")
	, m_salary(salary)
{
}

void MrBurns::AddEmployee(Actor* employee)
{
	m_employee.emplace(employee);
}

void MrBurns::DoAction(Bank& bank)
{
	const auto mrBurnsAccountId = GetAccountId();
	for (const auto& employee : m_employee)
	{
		const auto employeeAccountId = employee->GetAccountId();
		if (mrBurnsAccountId.has_value() && employeeAccountId.has_value())
		{
			try
			{
				bank.SendMoney(*mrBurnsAccountId, *employeeAccountId, m_salary);
				std::cout << GetName() << ": pay salary for " << employee->GetName() << std::endl;
			}
			catch (const std::exception& e)
			{
				std::cout << GetName() << ": " << e.what() << std::endl;
			}
		}
		else
		{
			std::cout << GetName() << ": didn't pay salary for " << employee->GetName() << std::endl;
		}
	}
}