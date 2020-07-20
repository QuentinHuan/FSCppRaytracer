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
#include "HitInfo.hpp"

class Box {
public:
	Box();
	Box(Vector3 min,Vector3 max);
	bool intersect(Ray &r);
	HitInfo intersectDebug(Ray &r);

	Vector3 min,max,center;

	Vector3 bounds[2];

	static Box boundingBox(std::vector<Box>* B);
};

#endif /* BOX_HPP_ */
