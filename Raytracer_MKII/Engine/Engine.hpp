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



class Engine {
private:

	//rendering properties
	int maxBounce;
	bool useAccelerationStructure = false;

	//random generator
	std::default_random_engine generator;
	std::uniform_real_distribution<float> distribution;

	//numericalConst
	const float selfIntersectionThreshold = 0.000001;

	//internal data structure
	Statistics *statCounter;
	std::vector<Object> & objectList;
	std::vector<Triangle> lightTriangleList;
	std::vector<HitInfo> camRayCache;

	//fonctions------------------------------------------------
	std::vector<HitInfo> buildIndirectLightStructure(Ray & camRay, HitInfo &cache);
	std::vector<HitInfo> buildDirectLightStructure(Ray & camRay, HitInfo &cache);

	void bounceRay(HitInfo hit, Ray r, int bounce, bool directLight, std::vector<HitInfo> &structure);
	Color computeLightAlongRay(Ray &camRay,HitInfo &cache);

	HitInfo rayCast(Ray r);

	HitInfo intersectObject(Object &obj, Ray r);
	Ray generateShadowRay(Vector3 origin);
	Vector3 importanceSampling(Vector3 normal);
	Vector3 uniformRndInSolidAngle(Vector3 normal,float angle);
	Vector3 uniformRndInTriangle(Triangle t);
	Vector3 uniformRndInSphericalTriangle(Triangle t,Vector3 position);

	float triangleViewAngle(Triangle t, Vector3 viewerPosition);

public:
	//fonctions------------------------------------------------
	Engine();
	Engine(std::vector<Object> & objectList, Statistics &statCounter,int maxBounce);
	Color rayTrace(Ray camRay, HitInfo &cache);
	HitInfo buildCache(Ray r);

};

#endif /* ENGINE_HPP_ */
