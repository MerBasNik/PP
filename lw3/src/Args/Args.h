#ifndef ECONOMY_ARGS_H
#define ECONOMY_ARGS_H

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
