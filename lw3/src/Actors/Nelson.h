//
// Created by Вадим Патрушев on 31.03.2026.
//

#ifndef ECONOMY_NELSON_H
#define ECONOMY_NELSON_H
#include "Actor.h"
#include "Apu.h"
#include "Bart.h"

class Nelson : public Actor
{
public:
	explicit Nelson(Money stealMoney);
	void DoAction(Bank& bank) override;
	void SetApu(Apu* apu);
	void SetBart(Bart* bart);
	void StealMoney();
	void BuyPurchase();

private:
	Money m_stealMoney;
	Bart* m_bart = nullptr;
	Apu* m_apu = nullptr;
};

#endif // ECONOMY_NELSON_H
