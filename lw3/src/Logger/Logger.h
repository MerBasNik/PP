#ifndef ECONOMY_LOGGER_H
#define ECONOMY_LOGGER_H
#include <iostream>
#include <mutex>
#include <string_view>

class Logger
{
public:
	void Print(const std::string_view line)
	{
		std::lock_guard lock(m_mutex);
		std::cout << line << '\n';
	}

private:
	std::mutex m_mutex;
};

#endif // ECONOMY_LOGGER_H
