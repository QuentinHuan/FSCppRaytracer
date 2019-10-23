/*
 * HitInfo.cpp
 *
 *  Created on: Sep 14, 2019
 *      Author: huan
 */

#include "HitInfo.hpp"

HitInfo::HitInfo() {
	this->r = Ray();
	this->intersection = 0;
	this->material = Material();
	this->normal = Vector3();
}

HitInfo::HitInfo(Ray r, float intersection, Material material, Vector3 normal) {
	this->r = r;
	this->intersection = intersection;
	this->material = material;
	this->normal = normal;
}


HitInfo HitInfo::sortForeground(std::vector<HitInfo> array) {

	if(array.size() != 0)
	{
		HitInfo foreground = array.at(0);

		for(HitInfo t : array)
		{
			if(t.intersection< foreground.intersection)
			{
				foreground = t;
			}
		}
		return foreground;
	}
	else
	{
		return HitInfo();
	}
}

Vector3 HitInfo::calcIntersectionCoord() {
	return r.pos + (r.dir * intersection);
	//return intersection;
}
