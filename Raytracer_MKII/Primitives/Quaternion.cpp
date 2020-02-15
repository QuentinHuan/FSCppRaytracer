/*
 * Quaternion.cpp
 *
 *  Created on: Feb 15, 2020
 *      Author: huan
 */

#include <Quaternion.hpp>

Quaternion::Quaternion() : a(0), v() {}

Quaternion::Quaternion(float a, Vector3 v) : a(a), v(v) {}

Quaternion Quaternion::operator +(const Quaternion &q) {
	return Quaternion(a + q.a, v+q.v);
}

Quaternion Quaternion::operator -(const Quaternion &q) {
	return Quaternion(a - q.a, v-q.v);
}

bool Quaternion::operator ==(Quaternion q) {
	if(a == q.a && v == q.v) return true;
	else return false;
}

Quaternion Quaternion::operator *(Quaternion q) {

	return Quaternion(a*q.a - Vector3::dot(v,q.v), q.v*a + v*q.a + Vector3::cross(v,q.v));
}

Quaternion Quaternion::inverse() {
	return Quaternion(a,v*(-1))*(a*a + Vector3::dot(v,v));
}

Quaternion Quaternion::operator *(float f) {
	return Quaternion(a*f,v*f);
}

Vector3 Quaternion::rotate(float angle, Vector3 axis, Vector3 vec) {

	Quaternion q = Quaternion(std::cos(angle/2), axis.normalize()*std::sin(angle/2));
	Quaternion V = Quaternion(0,vec);

	return (q*V*q.inverse()).v;

}
