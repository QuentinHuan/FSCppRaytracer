/*
 * Vector3.hpp
 *
 *  Created on: Sep 1, 2019
 *      Author: huan
 */
#pragma once
#ifndef VECTOR3_HPP_
#define VECTOR3_HPP_
#include <math.h>
#include "Statistics.hpp"
#include "Utility.hpp"

class Vector3
{

public:
	float x,y,z;

	Vector3();
	Vector3(float x, float y, float z);

	Vector3 operator +(Vector3 v);
	Vector3 operator -(Vector3 v);
	bool operator ==(Vector3 v);
	Vector3 operator *(const float lf);
	Vector3 operator /(const float lf);
	static float dot(Vector3 u, Vector3 v);
	static Vector3 cross(Vector3 u, Vector3 v);
	static float calcNorm(Vector3 v);

	Vector3 normalize();
};



#endif /* VECTOR3_HPP_ */
