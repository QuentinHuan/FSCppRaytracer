/*
 * Statistics.hpp
 *
 *  Created on: Sep 26, 2019
 *      Author: huan
 */

#pragma once
#ifndef STATISTICS_HPP_
#define STATISTICS_HPP_
#include <chrono>
#include <vector>
#include <string>

class Statistics {
	private:
	struct criteria
	{
		std::string name;
		double execTime=0;
		int occurence=0;
	};
	std::vector<criteria> criteriaList;

public:

	Statistics();
	double runtime;


	void addCriteriaOccurence(int id);
	void addCriteriaTime(int id, double timeOfExecution);
	std::string toString();

};

#endif /* STATISTICS_HPP_ */
