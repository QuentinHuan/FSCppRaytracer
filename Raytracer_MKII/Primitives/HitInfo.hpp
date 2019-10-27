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
#include "Object.hpp"
#include "Material.hpp"

class HitInfo {
public:
	float intersection;
	bool hitSomething;
	Ray r;
	Vector3 normal;
	Material material;
	HitInfo();
	HitInfo(Ray r, float intersection, Material material, Vector3 normal);

	static HitInfo sortForeground(std::vector<HitInfo> array);
	Vector3 calcIntersectionCoord();

};

#endif /* HITINFO_HPP_ */
