/*
 * Timer.hpp
 *
 *  Created on: Sep 27, 2019
 *      Author: huan
 */
#pragma once
#ifndef TIMER_HPP_
#define TIMER_HPP_

#include <chrono> // for std::chrono functions

class Timer {
	private:
	// Type aliases to make accessing nested type easier
	using clock_t = std::chrono::high_resolution_clock;
	using second_t = std::chrono::duration<double, std::ratio<1> >;

	std::chrono::time_point<clock_t> m_beg;

public:
	Timer();
	void reset();
	double elapsed();


};

#endif /* TIMER_HPP_ */
