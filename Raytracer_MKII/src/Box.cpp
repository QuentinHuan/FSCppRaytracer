/*
 * Box.cpp
 *
 *  Created on: Oct 5, 2019
 *      Author: huan
 */

#include "Box.hpp"


Box::Box(Vector3 min, Vector3 max) {
	this->min = min;
	this->max = max;
}

Box::Box() {
	min = Vector3();
	max = Vector3();
}
