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
#include "Utility.hpp"

class Vector3
{

public:
	float x, y, z;

	Vector3() : x(0), y(0), z(0) {}
	Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

	Vector3 operator+(const Vector3 &v)
	{
		return Vector3(x + v.x, y + v.y, z + v.z);
	}

	Vector3 operator-(const Vector3 &v)
	{
		return Vector3(x - v.x, y - v.y, z - v.z);
	}

	Vector3 operator*(float lf)
	{
		return Vector3(lf * x, lf * y, lf * z);
	}

	Vector3 operator/(const float lf)
	{
		return Vector3(x / lf, y / lf, z / lf);
	}

	static float dot(const Vector3 &u, const Vector3 &v)
	{
		return u.x * v.x + u.y * v.y + u.z * v.z;
	}

	static Vector3 cross(const Vector3 &u, const Vector3 &v)
	{
		return Vector3(u.y * v.z - u.z * v.y, u.z * v.x - u.x * v.z, u.x * v.y - u.y * v.x);
	}

	static float calcNorm(Vector3 v)
	{
		return sqrt(dot(v, v));
	}

	bool operator==(Vector3 v)
	{
		if (x == v.x && y == v.y && z == v.z)
			return true;
		else
			return false;
	}

	float maxComponent()
	{
		return fmax(z, fmax(x, y));
	}

	float minComponent()
	{
		return fmin(z, fmin(x, y));
	}

	Vector3 operator*(Vector3 v)
	{
		return Vector3(x * v.x, y * v.y, z * v.z);
	}

	Vector3 normalize()
	{
		float n = calcNorm(*this);
		if (n != 0)
		{
			return *this / n;
		}
		else
		{
			return Vector3(0, 0, 0);
		}
	}

	static Vector3 min(Vector3 a, Vector3 b)
	{
		return Vector3(fmin(a.x, b.x), fmin(a.y, b.y), fmin(a.z, b.z));
	}

	static Vector3 max(Vector3 a, Vector3 b)
	{
		return Vector3(fmax(a.x, b.x), fmax(a.y, b.y), fmax(a.z, b.z));
	}

	float getComponent(int index)
	{
		switch (index)
		{
		case 1:
			return x;
			break;

		case 2:
			return y;
			break;

		case 3:
			return z;
			break;

		default:
			return 0;
			break;
		}
	}
};

#endif /* VECTOR3_HPP_ */
