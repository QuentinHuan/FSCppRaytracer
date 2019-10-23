/*
 * Box.hpp
 *
 *  Created on: Oct 5, 2019
 *      Author: huan
 */
#pragma once
#ifndef BOX_HPP_
#define BOX_HPP_
#include "Vector3.hpp"

class Box {
public:
	Box();
	Box(Vector3 min,Vector3 max);
	Vector3 min,max;
};

#endif /* BOX_HPP_ */
