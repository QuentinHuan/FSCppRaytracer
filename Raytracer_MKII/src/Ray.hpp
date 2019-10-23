/*
 * Ray.hpp
 *
 *  Created on: Sep 1, 2019
 *      Author: huan
 */
#pragma once
#ifndef RAY_HPP_
#define RAY_HPP_
#include "Vector3.hpp"

class Ray {
public:
	Vector3 dir,pos;
	Ray();
	Ray(Vector3 dir, Vector3 pos);
};

#endif /* RAY_HPP_ */
