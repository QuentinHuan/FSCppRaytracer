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
#include "Ray.hpp"

class Box {
public:
	Box();
	Box(Vector3 min,Vector3 max);
	bool intersect(const Ray &r, float &t);

	Vector3 min,max,center;
};

#endif /* BOX_HPP_ */
