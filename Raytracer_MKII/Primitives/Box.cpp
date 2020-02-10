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

Box Box::boundingBox(std::vector<Box> B) {

	//max
	Vector3 vMax = B.at(0).max;
	Vector3 vMin = B.at(0).min;

	std::vector<float> X;
	std::vector<float> Y;
	std::vector<float> Z;


	for(auto it = B.begin(); it != B.end();it++)
	{
		X.push_back(it->max.x);X.push_back(it->min.x);
		Y.push_back(it->max.y);Y.push_back(it->min.y);
		Z.push_back(it->max.z);Z.push_back(it->min.z);
	}

	for(int i = 0; i< B.size();i++)
	{
		if(vMin.x < X.at(i)) vMin.x = X.at(i);
		if(vMin.y < Y.at(i)) vMin.y = Y.at(i);
		if(vMin.z < Z.at(i)) vMin.z = Z.at(i);

		if(vMax.x < X.at(i)) vMax.x = X.at(i);
		if(vMax.y < Y.at(i)) vMax.y = Y.at(i);
		if(vMax.z < Z.at(i)) vMax.z = Z.at(i);
	}

	return Box(vMin,vMax);

}
