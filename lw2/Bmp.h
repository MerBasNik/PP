//
// Created by Вадим Патрушев on 18.10.2026.
//

#ifndef BMP_H
#define BMP_H

#include "Types/Image.h"
#include <string>

class Bmp
{
public:
	static Image ReadBmp(const std::string& path);
	static void WriteBmp(const std::string& path, const Image& image);
};


#endif // BMP_H