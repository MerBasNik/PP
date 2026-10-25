#include "Args.h"
#include <charconv>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>

constexpr std::string_view USAGE = "bank --mode <single|parallel> --seconds <N> [--quiet]";

[[noreturn]] static void ThrowUsage(const std::string& reason)
{
	throw std::invalid_argument(reason + "\nUsage: " + std::string(USAGE));
}

static Mode ParseMode(const std::string_view value)
{
	if (value == "single")
	{
		return Mode::Single;
	}
	if (value == "parallel")
	{
		return Mode::Parallel;
	}
	ThrowUsage("unknown mode: " + std::string(value));
}

static int ParseSeconds(const std::string_view value)
{
	int seconds = 0;
	const auto [end, error] = std::from_chars(value.data(), value.data() + value.size(), seconds);
	if (error != std::errc{} || end != value.data() + value.size() || seconds <= 0)
	{
		ThrowUsage("seconds must be a positive integer: " + std::string(value));
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
		if (arg == "--quiet")
		{
			quiet = true;
			continue;
		}
		if (arg != "--mode" && arg != "--seconds")
		{
			ThrowUsage("unknown argument: " + std::string(arg));
		}
		if (i + 1 >= argc)
		{
			ThrowUsage("missing value for " + std::string(arg));
		}

		const std::string_view value = argv[++i];
		if (arg == "--mode")
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
		ThrowUsage("--mode and --seconds are required");
	}

	return Args{ *mode, *seconds, quiet };
}
