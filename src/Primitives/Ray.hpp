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
	Ray() : dir(Vector3()), pos(Vector3()), invPDF(0) {}

	Ray(Vector3 dir, Vector3 pos) : dir(dir), pos(pos)
	{
		invDir = Vector3(1.0 / dir.x, 1.0 / dir.y, 1.0 / dir.z);
		sign[0] = (invDir.x < 0);
		sign[1] = (invDir.y < 0);
		sign[2] = (invDir.z < 0);
		invPDF = 0;
	}

	Ray(Vector3 dir, Vector3 pos, float invPDF) : dir(dir), pos(pos), invPDF(invPDF)
	{
		invDir = Vector3(1.0 / dir.x, 1.0 / dir.y, 1.0 / dir.z);
		sign[0] = (invDir.x < 0);
		sign[1] = (invDir.y < 0);
		sign[2] = (invDir.z < 0);
	}
};

#endif /* RAY_HPP_ */
