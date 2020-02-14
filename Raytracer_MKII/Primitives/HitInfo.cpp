/*
 * HitInfo.cpp
 *
 *  Created on: Sep 14, 2019
 *      Author: huan
 */

#include "HitInfo.hpp"

HitInfo::HitInfo() : r(Ray()), intersection(0), material(Material()), normal(Vector3()),hitSomething(false){}

HitInfo::HitInfo(Ray r, float intersection, Material material, Vector3 normal) : r(r), intersection(intersection), material(material), normal(normal),hitSomething(true){}


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
