//
// Created by Вадим Патрушев on 18.10.2026.
//

#ifndef IMAGE_H
#define IMAGE_H

#include <cstdint>
#include <vector>

class Image
{
public:
	Image(const int width, const int height, const int channels)
		: width(width)
		, height(height)
		, channels(channels)
		, pixels(static_cast<std::size_t>(width) * height * channels)
	{
	}
	Image() = default;

	std::uint8_t* Pixel(const int x, const int y)
	{
		return pixels.data() + (static_cast<std::size_t>(y) * width + x) * channels;
	}

	const std::uint8_t* Pixel(const int x, const int y) const
	{
		return pixels.data() + (static_cast<std::size_t>(y) * width + x) * channels;
	}

	int width = 0;
	int height = 0;
	int channels = 0;
	std::vector<std::uint8_t> pixels;
};

#endif // IMAGE_H
