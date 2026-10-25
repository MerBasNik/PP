#ifndef ECONOMY_CUSTOMER_H
#define ECONOMY_CUSTOMER_H
#include "Actor.h"

class Customer : public Actor
{
public:
	Customer(std::string name, Money purchase);
	void DoAction(Bank& bank) override;
	void SetApu(Actor* apu);

private:
	Money m_purchase;
	Actor* m_apu = nullptr;
};

#endif // ECONOMY_CUSTOMER_H
