/*
 * Statistics.cpp
 *
 *  Created on: Sep 26, 2019
 *      Author: huan
 */

#include "Statistics.hpp"

Statistics::Statistics() {
	runtime = 0;
	criteriaList = std::vector<criteria>();

	criteria c0;
	c0.name = "Number of camera rays";

	criteria c1;
	c1.name = "DEBUG";

	criteria c2;
	c2.name = "rays not hitting a light";

	criteria c3;
	c3.name = "rays not hitting anything";

	criteria c4;
	c4.name = "number of shadow Rays";

	criteriaList.push_back(c0);
	criteriaList.push_back(c1);
	//criteriaList.push_back(c2);
	//criteriaList.push_back(c3);
	//criteriaList.push_back(c4);

}

void Statistics::addCriteriaOccurence(int id) {

	criteriaList.at(id).occurence++;
}

void Statistics::addCriteriaTime(int id, double timeOfExecution) {
	criteriaList.at(id).occurence++;
	criteriaList.at(id).execTime +=  timeOfExecution;
}

std::string Statistics::toString(bool verbose) {
	std::string s = "-----------------------\n";
	s += "Statistical report: \n";
	s += "-----------------------\n\n";
	s += "runtime = " + std::to_string(runtime) + "s\n\n";

	if(verbose)
	{
		for(int i=0;i<criteriaList.size();i++)
		{
			auto c = criteriaList.at(i);
			if(i != 0)
			{
				s += "-----------------------\n";
				s += c.name + " :\n";
				s += std::to_string(c.execTime) + "s\n";
			}
			else
			{
				s += "number ray cast: " + std::to_string(c.occurence) + "\n";
			}
		}
	}

	return s;
}
