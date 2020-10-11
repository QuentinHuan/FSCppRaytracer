/*
 * Color.h
 *
 *  Created on: Sep 1, 2019
 *      Author: huan
 */
#pragma once
#ifndef COLOR_HPP_
#define COLOR_HPP_

class Color
{
private:
public:
	float r, g, b;

	Color() : r(0), g(0), b(0) {}

	Color(float r, float g, float b) : r(r), g(g), b(b) {}

	void applyBounds()
	{
		if (r > 1)
			r = 1;
		if (g > 1)
			g = 1;
		if (b > 1)
			b = 1;

		if (r < 0)
			r = 0;
		if (g < 0)
			g = 0;
		if (b < 0)
			b = 0;
		return;
	}

	Color operator+(Color c)
	{
		Color result;
		result.r = this->r + c.r;
		result.g = this->g + c.g;
		result.b = this->b + c.b;
		return result;
	}

	Color operator-(Color c)
	{
		Color result;
		result.r = this->r - c.r;
		result.g = this->g - c.g;
		result.b = this->b - c.b;
		return result;
	}

	Color operator*(Color c)
	{
		Color result;
		result.r = this->r * c.r;
		result.g = this->g * c.g;
		result.b = this->b * c.b;
		return result;
	}

	Color operator/(Color c)
	{
		Color result;
		result.r = this->r / c.r;
		result.g = this->g / c.g;
		result.b = this->b / c.b;
		return result;
	}

	Color operator*(float f)
	{
		Color result;
		result.r = this->r * f;
		result.g = this->g * f;
		result.b = this->b * f;
		return result;
	}

	bool operator==(Color c)
	{
		if (r == c.r && b == c.b && g == c.g)
			return true;
		else
			return false;
	}
};

#endif /* COLOR_HPP_ */
