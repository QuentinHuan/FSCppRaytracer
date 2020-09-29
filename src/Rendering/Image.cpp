/*
 * Image.cpp
 *
 *  Created on: Sep 1, 2019
 *      Author: huan
 */

#include "Image.hpp"
#include <cmath>

Image::Image()
{
	// TODO Auto-generated constructor stub
	resX = 1;
	resY = 1;
	array.push_back(Color{0, 0, 0});
}

Image::Image(int resX, int resY)
{
	this->resX = resX;
	this->resY = resY;
	array.assign(resX * resY, Color{0, 0, 0});
}

Image::Image(int resX, int resY, std::vector<Color> sourceArray)
{
	this->resX = resX;
	this->resY = resY;
	array = sourceArray;
}

void Image::exportPPM(std::string file, int bitColorDepth)
{
	std::ofstream fichier{file};

	struct colorInt
	{
		int r, g, b;
	};

	std::vector<colorInt> img;
	int maxValue = pow(2, bitColorDepth) - 1;

	for (auto i = 0; i < array.size(); i++)
	{
		colorInt c;
		Color cf = array.at(i);
		cf.applyBounds();
		c.r = (int)(cf.r * maxValue);
		c.g = (int)(cf.g * maxValue);
		c.b = (int)(cf.b * maxValue);

		img.push_back(c);
	}

	//Header
	fichier << "P3" << std::endl;											   //magic number
	fichier << std::to_string(resX) + " " + std::to_string(resY) << std::endl; //resolution
	fichier << std::to_string(maxValue) << std::endl;						   //color depth

	//data
	for (int i = 0; i < resY; i++) //ligne
	{
		std::string ligne = "";
		for (int j = 0; j < resY; j++)
		{
			int index = j * resX + i;
			ligne += std::to_string(img.at(index).r) + " " + std::to_string(img.at(index).g) + " " + std::to_string(img.at(index).b) + "  ";
		}

		fichier << ligne << std::endl;
	}
}

float Image::gamma(float c)
{

	return std::pow(c, 1 / 2.2);
}
