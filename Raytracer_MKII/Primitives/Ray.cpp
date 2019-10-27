/*
 * Ray.cpp
 *
 *  Created on: Sep 1, 2019
 *      Author: huan
 */

#include "Ray.hpp"

Ray::Ray() {
	// TODO Auto-generated constructor stub
	this->dir = Vector3 {0,0,0};
	this->pos = Vector3 {0,0,0};
}

Ray::Ray(Vector3 dir, Vector3 pos) {
	this->dir = dir;
	this->pos = pos;
	invDir = Vector3();
	invDir.x = 1/dir.x;
	invDir.y = 1/dir.y;
	invDir.z = 1/dir.z;
}
