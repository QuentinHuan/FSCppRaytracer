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

class Image {
public:
	std::vector<Color> array;
	int resX,resY;
	Image();
	Image(int resX,int resY);
	Image(int resX,int resY, std::vector<Color> array);

	void exportPPM(std::string file,int bitColorDepth);
};

#endif /* IMAGE_HPP_ */
