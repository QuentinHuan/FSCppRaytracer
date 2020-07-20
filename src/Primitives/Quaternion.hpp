/*
 * Quaternion.hpp
 *
 *  Created on: Feb 15, 2020
 *      Author: huan
 */
#pragma once
#ifndef QUATERNION_HPP_
#define QUATERNION_HPP_
#include "Vector3.hpp"

class Quaternion {
public:

	float a;
	Vector3 v;

	Quaternion();
	Quaternion(float a,Vector3 v);

	Quaternion operator +(const Quaternion & q);
	Quaternion operator -(const Quaternion & q);
	bool operator ==(Quaternion q);
	Quaternion operator *(Quaternion q);
	Quaternion operator *(float  f);
	Quaternion inverse ();

	static Vector3 rotate(float angle, Vector3 axis, Vector3 vec);
};

#endif /* QUATERNION_HPP_ */
