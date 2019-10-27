/*
 * Box.cpp
 *
 *  Created on: Oct 5, 2019
 *      Author: huan
 */

#include "Box.hpp"


Box::Box(Vector3 min, Vector3 max) {
	this->min = min;
	this->max = max;

	center = (max-min) / 2;
}

Box::Box() {
	min = Vector3();
	max = Vector3();
	center = Vector3();
}

bool Box::intersect(const Ray &r, float &t)
 {
       	float tmin = (min.x - r.pos.x) * r.invDir.x;
       	float tmax = ( max.x - r.pos.x) * r.invDir.x;

       	if (tmin > tmax) std::swap(tmin, tmax);

       	float tymin = ( min.y - r.pos.y) * r.invDir.y;
       	float tymax = ( max.y - r.pos.y) *  r.invDir.y;

       	if (tymin > tymax) std::swap(tymin, tymax);

       	if ((tmin > tymax) || (tymin > tmax))
       		return false;

       	if (tymin > tmin)
       		tmin = tymin;

       	if (tymax < tmax)
       		tmax = tymax;

       	float tzmin = ( min.z - r.pos.z) * r.invDir.z;
       	float tzmax = ( max.z - r.pos.z) * r.invDir.z;

       	if (tzmin > tzmax) std::swap(tzmin, tzmax);

       	if ((tmin > tzmax) || (tzmin > tmax))
       		return false;

       	if (tzmin > tmin)
       		tmin = tzmin;

       	if (tzmax < tmax)
       		tmax = tzmax;

       t = tmin;

       if (t < 0) {
           t = tmax;
           if (t < 0) return false;
       }

       return true;
 }

