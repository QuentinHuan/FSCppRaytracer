/*
 * Triangle.cpp
 *
 *  Created on: Sep 1, 2019
 *      Author: huan
 */

#include "Triangle.hpp"

Triangle::Triangle() {
	this->a = Vector3();
	this->b = Vector3();
	this->c = Vector3();
	this->normal = Vector3();
	this->material = Material();
}

Triangle::Triangle(Vector3 a, Vector3 b, Vector3 c, Material material) {
	this->a = a;
	this->b = b;
	this->c = c;
	this->normal = calcNormal();
	this->material = material;
}


Triangle::Triangle(Vector3 a, Vector3 b, Vector3 c, Vector3 normal, Material material) {
	this->a = a;
	this->b = b;
	this->c = c;
	this->normal = normal;
	this->material = material;
}


Vector3 Triangle::calcNormal() {
	return Vector3::cross(a,b);
}

Vector3 Triangle::calcCenter() {
	return (a +b +c)/3.0;
}
