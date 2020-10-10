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

class Ray
{
public:
	Vector3 dir, pos;
	Vector3 invDir;
	int sign[3];
	float invPDF;
	Ray();
	Ray(Vector3 dir, Vector3 pos);
	Ray(Vector3 dir, Vector3 pos, float distributionSolidAngle);
};

#endif /* RAY_HPP_ */
