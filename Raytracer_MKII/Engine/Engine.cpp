/*
 * Engine.cpp
 *
 *  Created on: Sep 4, 2019
 *      Author: huan
 */

#include "Engine.hpp"

#include "HitInfo.hpp"


//-------------------------------
//Engine Construction
//-------------------------------
Engine::Engine(std::vector<Object> objectList, Statistics &statCounter,BSP &bsp,int maxBounce,bool useAccelerationStructure,bool useCache) {
	this->objectList = objectList;
	generator = std::default_random_engine();
	distribution = std::uniform_real_distribution<float>(0,1.0);
	this->statCounter = &statCounter;

	this->heuristic = heuristic;
	this->maxBounce = maxBounce;
	this->useCache = useCache;

	accelerationStructure=bsp;
	this->useAccelerationStructure = useAccelerationStructure;
	for(Object o : objectList)
	{
		for(Triangle t : o.faces)
		{
			if(t.material.emission)
			{
				lightTriangleList.push_back(t.calcCenter());
			}
		}
	}
	camRayCache = std::vector<HitInfo>();
}

HitInfo Engine::buildCache(Ray r) {

	return  rayCast(r);

}

//MAIN
Color Engine::rayTrace(Ray camRay, HitInfo &cache) {
	Color c = computeLightAlongRay(camRay,cache);
	return c;
}


//-------------------------------
//Ray tracing Logic
//-------------------------------

//_______________________________
//buildIntersectionStructure

//compute all the intersections from camera to the first light hit, bouncing off objects
std::vector<HitInfo> Engine::buildIntersectionStructure(Ray camRay, HitInfo &cache) {
	std::vector<HitInfo> structure;
	HitInfo hit{};
	int bounce = 0;

	//fire bounce rays until it reaches a light source
	statCounter->addCriteriaOccurence(0);//number of rays from the camera
	do
	{
		//-------------------------
		//Ray construction
		Ray r;
		if(structure.size() == 0) //first ray is the ray from the camera
		{
				r = camRay;
		}
		else
		{
			r = structure.at(structure.size()-1).r;//otherwise, we cast a ray starting from the intersection, in the direction computed during the last iteration
		}

		//-------------------------
		//Intersection Computation

		if(useCache && bounce == 0)//use cache for the camRay (same for each pass)
		{
			hit = cache;
		}
		else
		{
			hit = rayCast(r);
		}

		if(hit.hitSomething)//hit something: bounce the ray
		{
			bounceRay(hit,r,bounce,structure);
			bounce++;
		}
		else//hit nothing, stop
		{
			hit.r = Ray(Vector3(0,0,0),Vector3(0,0,0));
			hit.material = Material(Color(1,1,1),true,1.0);
			structure.push_back(hit);
			statCounter->addCriteriaOccurence(3);
			return structure;
		}


	}while(hit.material.emission == false && bounce <= maxBounce);
	return structure;
}


void Engine::bounceRay(HitInfo hit, Ray r, int bounce, std::vector<HitInfo> &structure) {


	if(hit.material.emission == true)//it's a light: stop
	{
		hit.r = Ray(Vector3(0,0,0),hit.calcIntersectionCoord());
		structure.push_back(hit);
		statCounter->addCriteriaOccurence(1);//number of rays hitting a light source
	}
	else//otherwise, bounce
	{
		if(bounce == maxBounce) statCounter->addCriteriaOccurence(2);//number of rays not finding a light source
		if(bounce == maxBounce-1 && heuristic)//not hitting anything after n bounces, last chance to hit a light
		{
			//the next bounce ray will not be random, but towards a light source
			Vector3 debug = hit.calcIntersectionCoord() + hit.normal*selfIntersectionThreshold;
			hit.r =generateShadowRay(debug);
			structure.push_back(hit);
		}
		else
		{
			hit.r = Ray(importanceSampling(hit.normal),hit.calcIntersectionCoord() + hit.normal*selfIntersectionThreshold);//new bounce ray from the intersection point (+ a small offset to prevent self intersecting artifacts)
			structure.push_back(hit);
		}
	}
	return;
}


//_______________________________
//computeLightAlongRay

//compute pixel color from the multiple data gathered by buildIntersectionStructure(...)
Color Engine::computeLightAlongRay(Ray camRay, HitInfo &cache) {
	//std::vector<HitInfo> structure = buildIntersectionStructure(camRay);
	std::vector<HitInfo> structure = buildIntersectionStructure(camRay, cache);

	Color output = Color(0,0,0);
	if(structure.size() > 0 && structure.at(structure.size()-1).r.dir == Vector3(0,0,0))
	{
		for(int i =structure.size()-1; i>=0; i--)//we travel across the structure backward
		{
			//last ray hit a light
			if(i==structure.size()-1) output = structure.at(i).material.diffuse*structure.at(i).material.emissionPower;

			else//at each bounce, the light intensity is absorbed (material dependent)
			{
				//Vector3 v = structure.at(i).normal.normalize();
				//float lambert = Vector3::dot(v,structure.at(i).r.dir.normalize());//Absorption coef
				//output = output*structure.at(i).material.diffuse*abs(lambert) *2.0; //uniform distrib
				output = output*structure.at(i).material.diffuse; //cosinus distrib
				//output = structure.at(i).material.diffuse;
			}
		}
	}
	return output;
}

//-------------------------------
//Geometry
//-------------------------------

//PUBLIC:------------------------------

//___________________
//rayCast

//perform a ray cast in the scene against all objects
//return true if the ray hit something
//all useful info (coord, material ect...) are stored in the HitInfo object provided
HitInfo Engine::rayCast(Ray r) {
	std::vector<HitInfo> allHit;

	for(Object o : objectList)
	{

		HitInfo hit = intersectObject(o,r);
		if(hit.hitSomething)
		{
			allHit.push_back(hit);
		}
	}

	if(allHit.size() != 0)
	{
		return HitInfo::sortForeground(allHit);
	}
	else
	{
		return HitInfo();
	}
}

//PRIVATE:----------------------------

//___________________
//intersectObject

//perform a ray cast against one object
//return true if the ray hit something
//all useful info (coord, material ect...) are stored in the HitInfo object provided
HitInfo Engine::intersectObject(Object obj, Ray r) {
	std::vector<HitInfo> allHit;

	if(useAccelerationStructure)
	{
		obj.faces = accelerationStructure.accelerationStructure(r);
	}

	//fill the vector with the coord of all intersection points (in ray space)
	for(auto tri : obj.faces)
	{
		HitInfo h{};
		h = tri.intersect(r);
		if(h.hitSomething)
		{
			allHit.push_back(h);
		}
	}
	if(allHit.size() > 0)
	{
		return HitInfo::sortForeground(allHit);
	}
	else
	{
		return HitInfo();
	}
}

//___________________
//importanceSampling

Vector3 Engine::importanceSampling(Vector3 normal) {

	float u = distribution(generator);
	float theta = distribution(generator);


	float Phi = std::asin(std::pow(u,0.5));//distribution: change pow to get various cos^n distrib over the sphere (pow=1 means uniform)


	Vector3 localV = Vector3( std::sin(Phi)*std::cos(theta),std::sin(Phi)*std::sin(theta),std::cos(Phi) );

	Vector3 Z = normal;
	Vector3 X;
	Vector3 Y;

	if(Z.y != 0) Y = Vector3(Z.y,-(Z.x + Z.z),Z.y);
	else  Y = Vector3(Z.z,0,-Z.x);

	X = Vector3::cross(Y,Z);
	Vector3 worldV =  X*localV.x + Y*localV.y + Z*localV.z;

	return worldV;
}



//___________________
//generateShadowRay

//A modifier pour plusieur lumière;
Ray Engine::generateShadowRay(Vector3 origin) {

	//Vector3 n = importanceSampling(Vector3(1,0,0),360);
	Vector3 direction = (lightTriangleList.at(0) - origin).normalize();
	Ray r = Ray(importanceSampling(direction),origin);
	return r;
}


