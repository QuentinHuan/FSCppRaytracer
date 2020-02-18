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
	return Vector3(lf*x,lf*y,lf*z);
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

float Vector3::maxComponent() {
	return fmax(z,fmax(x,y));
}

float Vector3::minComponent() {
	return fmin(z,fmin(x,y));
}

Vector3 Vector3::operator *(Vector3 v) {
	return Vector3(x*v.x,y*v.y,z*v.z);
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

Vector3 Vector3::min(Vector3 a, Vector3 b) {
	return Vector3(fmin(a.x,b.x),fmin(a.y,b.y),fmin(a.z,b.z));
}

Vector3 Vector3::max(Vector3 a, Vector3 b) {
	return Vector3(fmax(a.x,b.x),fmax(a.y,b.y),fmax(a.z,b.z));
}
