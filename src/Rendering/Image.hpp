/*
 * Image.h
 *
 *  Created on: Sep 1, 2019
 *      Author: huan
 */
#pragma once
#ifndef IMAGE_HPP_
#define IMAGE_HPP_
#include <iostream>
#include <fstream>
#include "Color.hpp"
#include <vector>

class Image
{
public:
	std::vector<Color> array;
	int resX, resY;
	Image()
	{
		// TODO Auto-generated constructor stub
		resX = 1;
		resY = 1;
		array.push_back(Color{0, 0, 0});
	}

	Image(int resX, int resY)
	{
		this->resX = resX;
		this->resY = resY;
		array.assign(resX * resY, Color{0, 0, 0});
	}

	Image(int resX, int resY, std::vector<Color> sourceArray)
	{
		this->resX = resX;
		this->resY = resY;
		array = sourceArray;
	}

	void exportPPM(std::string file, int bitColorDepth)
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

			//cf.applyBounds();
			//cf = cf / (cf + Color(1, 1, 1));
			//gamma correction
			cf.r = powf(cf.r, 1.0 / 2.2);
			cf.g = powf(cf.g, 1.0 / 2.2);
			cf.b = powf(cf.b, 1.0 / 2.2);

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
};

#endif /* IMAGE_HPP_ */
