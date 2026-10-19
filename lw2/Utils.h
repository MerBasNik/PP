//
// Created by Вадим Патрушев on 18.10.2026.
//

#ifndef PP_UTILS_H
#define PP_UTILS_H
#include "Bmp.h"
#include "Types/Args.h"
#include "Types/Rect.h"
#include <algorithm>
#include <chrono>
#include <cstdint>
#include <format>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <thread>
#include <vector>

const int MAX_CHANNELS = 4;

void BlurPixel(const Image& src, Image& dst, const int x, const int y, const int radius)
{
	int sums[MAX_CHANNELS] = {};

	for (int dy = -radius; dy <= radius; ++dy)
	{
		for (int dx = -radius; dx <= radius; ++dx)
		{
			const int nx = std::clamp(x + dx, 0, src.width - 1);
			const int ny = std::clamp(y + dy, 0, src.height - 1);
			const std::uint8_t* neighbor = src.Pixel(nx, ny);

			for (int c = 0; c < src.channels; ++c)
			{
				sums[c] += neighbor[c];
			}
		}
	}

	const int side = 2 * radius + 1;
	const int count = side * side;
	std::uint8_t* out = dst.Pixel(x, y);

	for (int c = 0; c < src.channels; ++c)
	{
		out[c] = static_cast<std::uint8_t>((sums[c] + count / 2) / count);
	}
}

void BlurRect(const Image& src, Image& dst, const Rect rect, const int radius)
{
	for (int y = rect.y0; y < rect.y1; ++y)
	{
		for (int x = rect.x0; x < rect.x1; ++x)
		{
			BlurPixel(src, dst, x, y, radius);
		}
	}
}

void Worker(const Image& src, Image& dst, const std::vector<Rect>& rects, const int radius)
{
	for (const Rect& rc : rects)
	{
		BlurRect(src, dst, rc, radius);
	}
}

std::vector<std::vector<Rect>> MakeWork(const int width, const int height, const int threadCount)
{
	std::vector<std::vector<Rect>> work(threadCount);

	for (int k = 0; k < threadCount; ++k)
	{
		const auto y0 = k * height / threadCount;
		const auto y1 = (k + 1) * height / threadCount;
		if (y0 < y1)
		{
			work[k].push_back(Rect{ 0, y0, width, y1 });
		}
	}

	return work;
}

void WriteResult(const Args& args, const std::chrono::milliseconds workTime)
{
	std::cout << args.threads << ',' << args.radius << ',' << std::thread::hardware_concurrency()
		<< ',' << workTime.count() << std::endl;
}

void Run(const Args& args)
{
	const Image src = Bmp::ReadBmp(args.input);
	Image dst(src.width, src.height, src.channels);

	auto work = MakeWork(src.width, src.height, args.threads);
	const auto start = std::chrono::steady_clock::now();

	{
		std::vector<std::jthread> threads;
		threads.reserve(args.threads);
		for (int k = 0; k < args.threads; ++k)
		{
			threads.emplace_back(Worker, std::cref(src), std::ref(dst), std::move(work[k]), args.radius);
		}
	}

	const auto finish = std::chrono::steady_clock::now();
	const auto workTime = std::chrono::duration_cast<std::chrono::milliseconds>(finish - start);

	Bmp::WriteBmp(args.output, dst);
	WriteResult(args, workTime);
}

Args ParseArgs(const int argc, char* argv[])
{
	if (argc != 4 && argc != 5)
	{
		throw std::invalid_argument("blur <input.bmp> <output.bmp> <threads> <radius>");
	}

	Args args;
	args.input = argv[1];
	args.output = argv[2];

	const int threads = std::stoi(argv[3]);
	if (threads < MIN_THREADS || threads > MAX_THREADS)
	{
		throw std::invalid_argument(std::format("Число потоков должно быть от {} до {}", MIN_THREADS, MAX_THREADS));
	}
	args.threads = threads;

	if (argc == 5)
	{
		const int radius = std::stoi(argv[4]);
		if (radius < 0)
		{
			throw std::invalid_argument("Радиус должен быть положительным");
		}
		args.radius = radius;
	}

	return args;
}

#endif //PP_UTILS_H
