/*
 * HitInfo.hpp
 *
 *  Created on: Sep 14, 2019
 *      Author: huan
 */
#pragma once
#ifndef HITINFO_HPP_
#define HITINFO_HPP_
#include "Ray.hpp"
#include "Material.hpp"

class HitInfo
{
public:
	Ray r;
	float intersection;
	int material;
	Vector3 normal;

	bool hitSomething;

	HitInfo() : r(Ray()), intersection(0), material(0), normal(Vector3()), hitSomething(false) {}

	HitInfo(Ray r, float intersection, int material, Vector3 normal) : r(r), intersection(intersection), material(material), normal(normal), hitSomething(true) {}

	static HitInfo sortForeground(std::vector<HitInfo> &array)
	{
		if (array.size() != 0)
		{
			HitInfo foreground = array.at(0);
			for (auto it = array.begin(); it != array.end(); it++)
			{
				if (it->intersection < foreground.intersection)
					foreground = *it;
			}
			return foreground;
		}
		else
			return HitInfo();
	}

	Vector3 calcIntersectionCoord()
	{
		return r.pos + (r.dir * intersection);
	}
};

#endif /* HITINFO_HPP_ */
