//
// Created by Вадим Патрушев on 24.03.2026.
//

#ifndef ECONOMY_LISA_H
#define ECONOMY_LISA_H
#include "Customer.h"

class Lisa : public Customer
{
public:
	explicit Lisa(Money purchase);
};

#endif // ECONOMY_LISA_H
