/*
 * Color.h
 *
 *  Created on: Sep 1, 2019
 *      Author: huan
 */
#pragma once
#ifndef COLOR_HPP_
#define COLOR_HPP_

class Color {
private:

public:
	float r,g,b;
	Color();
	Color(float r, float g, float b);

	void applyBounds();
	Color operator +(Color c);
	Color operator -(Color c);
	Color operator *(Color c);
	Color operator *(float f);
	bool operator ==(Color c);
};

#endif /* COLOR_HPP_ */
