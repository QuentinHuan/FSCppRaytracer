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

class Quaternion
{
public:
	float a;
	Vector3 v;

	Quaternion() : a(0), v() {}

	Quaternion(float a, Vector3 v) : a(a), v(v) {}

	Quaternion operator+(const Quaternion &q)
	{
		return Quaternion(a + q.a, v + q.v);
	}

	Quaternion operator-(const Quaternion &q)
	{
		return Quaternion(a - q.a, v - q.v);
	}

	bool operator==(Quaternion q)
	{
		if (a == q.a && v == q.v)
			return true;
		else
			return false;
	}

	Quaternion operator*(Quaternion q)
	{

		return Quaternion(a * q.a - Vector3::dot(v, q.v), q.v * a + v * q.a + Vector3::cross(v, q.v));
	}

	Quaternion inverse()
	{
		return Quaternion(a, v * (-1)) * (a * a + Vector3::dot(v, v));
	}

	Quaternion operator*(float f)
	{
		return Quaternion(a * f, v * f);
	}

	static Vector3 rotate(float angle, Vector3 axis, Vector3 vec)
	{

		Quaternion q = Quaternion(std::cos(angle / 2), axis.normalize() * std::sin(angle / 2));
		Quaternion V = Quaternion(0, vec);

		return (q * V * q.inverse()).v;
	}
};

#endif /* QUATERNION_HPP_ */
