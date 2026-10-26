#include "Customer.h"

#include <format>

Customer::Customer(std::string name, const Money purchase)
	: Actor(std::move(name))
	, m_purchase(purchase)
{
}

void Customer::SetApu(Actor* apu)
{
	m_apu = apu;
}

void Customer::DoAction(Bank&)
{
	if (!m_apu)
	{
		return;
	}
	if (GiveCash(*m_apu, m_purchase))
	{
		const std::string message = std::format("bought something for {} in the {}'s store", m_purchase, m_apu->GetName());
		Print(message);
	}
	else
	{
		const std::string message = std::format("skips turn: not enough cash to buy for {}", m_purchase);
		Print(message);
	}
}
