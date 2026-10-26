#ifndef ECONOMY_ARGS_H
#define ECONOMY_ARGS_H
#include <string>

constexpr std::string PARAM_MODE = "--mode";
constexpr std::string PARAM_SECONDS = "--seconds";
constexpr std::string PARAM_QUIET = "--quiet";
constexpr std::string SINGLE_MODE = "single";
constexpr std::string PARALLEL_MODE = "parallel";

enum class Mode
{
	Single,
	Parallel,
};

struct Args
{
	Mode mode = Mode::Single;
	int seconds = 0;
	bool quiet = false;
};

Args ParseArgs(int argc, const char* const argv[]);

#endif // ECONOMY_ARGS_H
