/*
 * Engine.hpp
 *
 *  Created on: Sep 4, 2019
 *      Author: huan
 */
#pragma once
#ifndef ENGINE_HPP_
#define ENGINE_HPP_
#include "Object.hpp"
#include "Ray.hpp"
#include "Color.hpp"
#include "HitInfo.hpp"
#include <random>
#include <cmath>

#include "Statistics.hpp"
#include "Timer.hpp"
#include "Utility.hpp"
#include "BVH.hpp"
#include "Quaternion.hpp"


class Engine {
private:

	//internal data structure
	std::vector<Triangle> & TriangleList;
	std::vector<Triangle> & lightTriangleList;
	BVH bvh;
	Statistics &statCounter;



	//rendering properties
	int maxBounce;
	bool useAccelerationStructure = 0;
	bool debugBVH = 0;

	Material background;
	float backgroundPower = 0.1;

	//random generator
	std::default_random_engine generator;
	std::uniform_real_distribution<float> distribution;

	//numericalConst
	const float selfIntersectionThreshold = 0.000001;

	//fonctions------------------------------------------------
	HitInfo rayCast(Ray r);
	HitInfo intersect(Ray &r);


	Color directLight(Ray &r,HitInfo &cache);

	Color globalIllumination(Ray &r,HitInfo &cache);
	Color GIBounce(HitInfo &hit);
	Ray GIRay(HitInfo &hit);

	Ray shadowRay(HitInfo &hit);
	Ray cosineWeightedInSolidAngle(float angle,Vector3 direction, Vector3 position);
	Ray uniformInSolidAngle(float angle,Vector3 direction, Vector3 position);

	float triangleViewAngle(Triangle t, Vector3 viewerPosition);

public:
	//fonctions------------------------------------------------
	Engine(std::vector<Triangle> & triangleList,std::vector<Triangle> & lightTriangleList, Statistics &statCounter,int maxBounce, BVH bvh);
	Color rayTrace(Ray &camRay, HitInfo &cache);
	HitInfo buildCache(Ray &r);

};

#endif /* ENGINE_HPP_ */
