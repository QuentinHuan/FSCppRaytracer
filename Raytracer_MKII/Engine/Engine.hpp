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

#include "BSP.hpp"
#include "Statistics.hpp"
#include "Timer.hpp"
#include "Utility.hpp"


class Engine {
private:

	//rendering properties
	int maxBounce;
	bool heuristic;
	bool useAccelerationStructure ;
	bool useCache;

	//random generator
	std::default_random_engine generator;
	std::uniform_real_distribution<float> distribution;

	//numericalConst
	const float selfIntersectionThreshold = 0.001;

	//internal data structure
	Statistics *statCounter;
	std::vector<Object> objectList;
	std::vector<Vector3> lightTriangleList;
	std::vector<HitInfo> camRayCache;


	//fonctions------------------------------------------------
	std::vector<HitInfo> buildIntersectionStructure(Ray camRay, HitInfo &cache);
	void bounceRay(HitInfo hit, Ray r, int bounce, std::vector<HitInfo> &structure);
	Color computeLightAlongRay(Ray camRay,HitInfo &cache);

	HitInfo rayCast(Ray r);
	HitInfo intersectObject(Object obj, Ray r);
	HitInfo intersectTri(Triangle tri, Ray r);
	bool intersectTriMoller(Triangle tri, Ray r, HitInfo &hitInfo);
	Ray generateShadowRay(Vector3 origin);
	Vector3 importanceSampling(Vector3 normal);



	BSP accelerationStructure;

public:
	//fonctions------------------------------------------------
	Engine(std::vector<Object> objectList, Statistics &statCounter,BSP &bsp,int maxBounce,bool useAccelerationStructure,bool useCache);
	Color rayTrace(Ray camRay, HitInfo &cache);
	HitInfo buildCache(Ray r);

};

#endif /* ENGINE_HPP_ */
