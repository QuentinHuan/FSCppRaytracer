/*
 * Vector3.cpp
 *
 *  Created on: Sep 1, 2019
 *      Author: huan
 */
#include "Vector3.hpp"

Vector3::Vector3() : x(0),  y(0), z(0){}
Vector3::Vector3(float x, float y, float z) : x(x),y(y),z(z){}

Vector3 Vector3::operator +(const Vector3 & v)
{
	return Vector3(x+v.x, y+v.y, z+v.z);
}

Vector3 Vector3::operator -(const Vector3 & v)
{
	return Vector3(x-v.x, y-v.y, z-v.z);
}

Vector3 Vector3::operator *(float lf)
{
	Vector3 result;
	result.x = lf*x;
	result.y = lf*y;
	result.z = lf*z;

	return result;


}

Vector3 Vector3::operator /(const float lf)
{
	return Vector3(x/lf, y/lf, z/lf);;
}

float Vector3::dot(const Vector3 & u, const Vector3 & v) {
	return u.x*v.x + u.y*v.y + u.z*v.z;
}

Vector3 Vector3::cross(const Vector3 & u, const Vector3 & v)
{
	return Vector3(u.y*v.z - u.z*v.y, u.z*v.x - u.x*v.z, u.x*v.y - u.y*v.x);
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
	if(n != 0)
	{
		return Vector3(x/n,y/n,z/n);
	}
	else
	{
		return 	Vector3(0,0,0);
	}
}

Vector3 Vector3::rotate(Vector3 axis, float angle, Vector3 v) {




}
