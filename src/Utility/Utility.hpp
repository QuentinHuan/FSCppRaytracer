/*
 * Utility.hpp
 *
 *  Created on: Sep 4, 2019
 *      Author: huan
 */
#pragma once
#ifndef UTILITY_HPP_
#define UTILITY_HPP_
#include <string>
#include <vector>

class Utility
{
public:
	Utility() {}

	static size_t split(const std::string &txt, std::vector<std::string> &strs, char ch)
	{
		size_t pos = txt.find(ch);
		size_t initialPos = 0;
		strs.clear();

		// Decompose statement
		while (pos != std::string::npos)
		{
			strs.push_back(txt.substr(initialPos, pos - initialPos));
			initialPos = pos + 1;

			pos = txt.find(ch, initialPos);
		}

		// Add the last one
		strs.push_back(txt.substr(initialPos, std::min(pos, txt.size()) - initialPos + 1));

		return strs.size();
	}

	static float min(std::vector<float> array)
	{
		float r = array.at(0);
		for (float f : array)
		{
			if (f < r)
				r = f;
		}
		return r;
	}

	static float max(std::vector<float> array)
	{
		float r = std::abs(array.at(0));
		for (int i = 0; i < array.size(); i++)
		{
			if (std::abs(array.at(i)) > std::abs(r))
				r = std::abs(array.at(i));
		}
		return std::abs(r);
	}
};

#endif /* UTILITY_HPP_ */
