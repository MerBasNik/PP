//
// Created by Вадим Патрушев on 23.03.2026.
//

#ifndef ECONOMY_BANKOPERATIONERROR_H
#define ECONOMY_BANKOPERATIONERROR_H
#include <stdexcept>

class BankOperationError : public std::runtime_error
{
public:
	using runtime_error::runtime_error;
};

#endif // ECONOMY_BANKOPERATIONERROR_H
