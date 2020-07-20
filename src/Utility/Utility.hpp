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

class Utility {
public:
	Utility();
	static float randomFloat();
	static size_t split(const std::string &txt, std::vector<std::string> &strs, char ch);
	static float min(std::vector<float> array);
	static float max(std::vector<float> array);


};

#endif /* UTILITY_HPP_ */
