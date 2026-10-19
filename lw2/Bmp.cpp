//
// Created by Вадим Патрушев on 18.10.2026.
//

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>
#include "Bmp.h"
#include <algorithm>
#include <memory>
#include <stdexcept>

Image Bmp::ReadBmp(const std::string& path)
{
	int width = 0;
	int height = 0;
	int channels = 0;
	auto* pixels = stbi_load(path.c_str(), &width, &height, &channels, 0);
	if (!pixels)
	{
		throw std::runtime_error("не удалось прочитать " + path + ": " + stbi_failure_reason());
	}

	Image image(width, height, channels);
	std::copy(pixels, pixels + image.pixels.size(), image.pixels.begin());
	stbi_image_free(pixels);
	return image;
}

void Bmp::WriteBmp(const std::string& path, const Image& image)
{
	if (!stbi_write_bmp(path.c_str(), image.width, image.height, image.channels, image.pixels.data()))
	{
		throw std::runtime_error("не удалось записать файл " + path);
	}
}
