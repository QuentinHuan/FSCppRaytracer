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
#include "Utility.hpp"
#include "Color.hpp"
#include "HitInfo.hpp"
#include <random>
#include "Statistics.hpp"
#include "Timer.hpp"
#include "BSP.hpp"


class Engine {
private:

	std::default_random_engine generator;
	std::uniform_real_distribution<float> distribution;
	float selfIntersectionThreshold = 0.001;

	std::vector<HitInfo> buildIntersectionStructure(Ray camRay,int pixelIndex);
	Color computeLightAlongRay(Ray camRay,int pixelIndex);

	bool rayCast(Ray r, HitInfo & hitInfo);
	bool intersectObject(Object obj, Ray r, HitInfo &hitInfo);
	bool intersectTri(Triangle tri, Ray r, HitInfo &hitInfo);
	bool intersectTriMoller(Triangle tri, Ray r, HitInfo &hitInfo);

	Vector3 randDirection(Vector3 normal, float angle);

	BSP accelerationStructure;

public:
	int maxBounce;
	float kEpsilon = 0.001;
	bool heuristic = false;
	bool useAccelerationStructure = true;
	bool useCache = true;

	Statistics *statCounter;
	std::vector<Object> objectList;
	std::vector<Vector3> lightTriangleList;
	std::vector<HitInfo> camRayCache;

	Engine(std::vector<Object> objectList, Statistics &statCounter,BSP &bsp,int maxBounce,bool useAccelerationStructure);

	Color rayTrace(Ray camRay,int pixelIndex);

	Ray generateShadowRay(Vector3 origin);

};

#endif /* ENGINE_HPP_ */
