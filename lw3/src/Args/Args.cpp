#include "Args.h"
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>

static Mode ParseMode(const std::string& value)
{
	if (value == SINGLE_MODE)
	{
		return Mode::Single;
	}
	if (value == PARALLEL_MODE)
	{
		return Mode::Parallel;
	}
	throw std::invalid_argument("unknown mode: " + std::string(value));
}

static int ParseSeconds(const std::string& value)
{
	std::size_t parsedLength = 0;
	int seconds = 0;
	try
	{
		seconds = std::stoi(value, &parsedLength);
	}
	catch (const std::logic_error&)
	{
		seconds = 0;
	}
	if (parsedLength != value.size() || seconds <= 0)
	{
		throw std::invalid_argument("seconds must be a positive integer: " + value);
	}
	return seconds;
}

Args ParseArgs(const int argc, const char* const argv[])
{
	std::optional<Mode> mode;
	std::optional<int> seconds;
	bool quiet = false;

	for (int i = 1; i < argc; ++i)
	{
		const std::string_view arg = argv[i];
		if (arg == PARAM_QUIET)
		{
			quiet = true;
			continue;
		}
		if (arg != PARAM_MODE && arg != PARAM_SECONDS)
		{
			throw std::invalid_argument("unknown argument: " + std::string(arg));
		}
		if (i + 1 >= argc)
		{
			throw std::invalid_argument("missing value for " + std::string(arg));
		}

		const std::string value = argv[++i];
		if (arg == PARAM_MODE)
		{
			mode = ParseMode(value);
		}
		else
		{
			seconds = ParseSeconds(value);
		}
	}

	if (!mode || !seconds)
	{
		throw std::invalid_argument(PARAM_MODE + " and " + PARAM_SECONDS + " are required");
	}

	return Args{ *mode, *seconds, quiet };
}
