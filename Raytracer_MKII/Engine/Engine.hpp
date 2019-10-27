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

#include "BSP.hpp"
#include "Statistics.hpp"
#include "Timer.hpp"
#include "Utility.hpp"


class Engine {
private:

	//rendering properties
	int maxBounce;
	bool heuristic = false;
	bool useAccelerationStructure = true;
	bool useCache = false;

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
	std::vector<HitInfo> buildIntersectionStructure(Ray camRay);
	Color computeLightAlongRay(Ray camRay);

	bool rayCast(Ray r, HitInfo & hitInfo);
	bool intersectObject(Object obj, Ray r, HitInfo &hitInfo);
	bool intersectTri(Triangle tri, Ray r, HitInfo &hitInfo);
	bool intersectTriMoller(Triangle tri, Ray r, HitInfo &hitInfo);
	Ray generateShadowRay(Vector3 origin);
	Vector3 randDirection(Vector3 normal, float angle);

	BSP accelerationStructure;

public:
	//fonctions------------------------------------------------
	Engine(std::vector<Object> objectList, Statistics &statCounter,BSP &bsp,int maxBounce,bool useAccelerationStructure);
	Color rayTrace(Ray camRay);

};

#endif /* ENGINE_HPP_ */
