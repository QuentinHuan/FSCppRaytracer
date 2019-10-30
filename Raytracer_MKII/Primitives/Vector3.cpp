/*
 * Vector3.cpp
 *
 *  Created on: Sep 1, 2019
 *      Author: huan
 */
#include "Vector3.hpp"

Vector3::Vector3()
{
	this->x = 0;
	this->y = 0;
	this->z = 0;
}
Vector3::Vector3(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

Vector3 Vector3::operator +(Vector3 v)
{
	Vector3 result;
	result.x = x+v.x;
	result.y = y+v.y;
	result.z = z+v.z;

	return result;
}

Vector3 Vector3::operator -(Vector3 v)
{
	Vector3 result;
	result.x = x-v.x;
	result.y = y-v.y;
	result.z = z-v.z;

	return result;
}

Vector3 Vector3::operator *(float lf)
{
	Vector3 result;
	result.x = lf*x;
	result.y = lf*y;
	result.z = lf*z;

	return result;


}

Vector3 Vector3::operator /(float lf)
{
	Vector3 result;
	result.x = x/lf;
	result.y = y/lf;
	result.z = z/lf;

	return result;


}

float Vector3::dot(Vector3 u, Vector3 v) {
	return u.x*v.x + u.y*v.y + u.z*v.z;
}

Vector3 Vector3::cross(Vector3 u, Vector3 v)
{
	Vector3 result;
	result.x = u.y*v.z - u.z*v.y;
	result.y = u.z*v.x - u.x*v.z;
	result.z = u.x*v.y - u.y*v.x;

	return result;
}

float Vector3::calcNorm(Vector3 v) {
	return sqrt(dot(v,v));
}

bool Vector3::operator ==(Vector3 v) {
	if(x == v.x && y == v.y && z == v.z) return true;
	else return false;
}

Vector3 Vector3::normalize() {

	float n = calcNorm(Vector3(x,y,z));
	Vector3 result(0,0,0);
	if(n != 0)
	{
		result = Vector3(x/n,y/n,z/n);
	}

		return result;
}
