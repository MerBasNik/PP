#include "src/Args/Args.h"
#include <gtest/gtest.h>
#include <stdexcept>
#include <vector>

static Args Parse(std::vector<const char*> args)
{
	args.insert(args.begin(), "bank");
	return ParseArgs(static_cast<int>(args.size()), args.data());
}

TEST(ArgsTest, ParallelQuiet)
{
	const Args args = Parse({ "--mode", "parallel", "--seconds", "10", "--quiet" });
	EXPECT_EQ(args.mode, Mode::Parallel);
	EXPECT_EQ(args.seconds, 10);
	EXPECT_TRUE(args.quiet);
}

TEST(ArgsTest, SingleInAnyOrder)
{
	const Args args = Parse({ "--seconds", "3", "--mode", "single" });
	EXPECT_EQ(args.mode, Mode::Single);
	EXPECT_EQ(args.seconds, 3);
	EXPECT_FALSE(args.quiet);
}

TEST(ArgsTest, InvalidArgs)
{
	EXPECT_THROW(Parse({}), std::invalid_argument);
	EXPECT_THROW(Parse({ "--mode", "single" }), std::invalid_argument);
	EXPECT_THROW(Parse({ "--seconds", "1" }), std::invalid_argument);
	EXPECT_THROW(Parse({ "--mode", "foo", "--seconds", "1" }), std::invalid_argument);
	EXPECT_THROW(Parse({ "--mode", "single", "--seconds", "0" }), std::invalid_argument);
	EXPECT_THROW(Parse({ "--mode", "single", "--seconds", "1x" }), std::invalid_argument);
	EXPECT_THROW(Parse({ "--mode", "single", "--seconds" }), std::invalid_argument);
	EXPECT_THROW(Parse({ "--mode", "single", "--seconds", "1", "--extra" }), std::invalid_argument);
}
