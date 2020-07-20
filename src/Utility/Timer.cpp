/*
 * Timer.cpp
 *
 *  Created on: Sep 27, 2019
 *      Author: huan
 */

#include "Timer.hpp"



	Timer::Timer() {
		reset();
	}


	void Timer::reset()
	{
		m_beg = clock_t::now();
	}

	double Timer::elapsed()
	{
		return std::chrono::duration_cast<second_t>(clock_t::now() - m_beg).count();
}
