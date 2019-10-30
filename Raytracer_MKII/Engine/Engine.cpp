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
	lightTriangleList = std::vector<Triangle>();

	for(Object o : objectList)
	{
		for(Triangle t : o.faces)
		{
			if(t.material.emission)
			{
				lightTriangleList.push_back(t);
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
std::vector<HitInfo> Engine::buildIndirectLightStructure(Ray camRay, HitInfo &cache) {
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
			bounceRay(hit,r,bounce,false,structure);
			bounce++;
		}
		else//hit nothing, stop
		{
			hit.r = Ray(Vector3(0,0,0),Vector3(0,0,0));
			//hit.material = Material(Color(1,1,1),true,1);
			hit.material =lightTriangleList.at(0).material;
			hit.material.emissionPower = 1;
			structure.push_back(hit);
			statCounter->addCriteriaOccurence(3);
			return structure;
		}


	}while(hit.material.emission == false && bounce <= maxBounce);
	return structure;
}

//compute all the intersections from camera to the first light hit, bouncing off objects
std::vector<HitInfo> Engine::buildDirectLightStructure(Ray camRay, HitInfo &cache) {
	std::vector<HitInfo> structure;
	HitInfo hit{};
	int bounce = 0;
	int max = 1;

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
			bounceRay(hit,r,bounce,true,structure);
			bounce++;
		}
		else//hit nothing, stop
		{
			hit.r = Ray(Vector3(0,0,0),Vector3(0,0,0));
			hit.material = Material(Color(1,1,1),true,0);
			structure.push_back(hit);
			statCounter->addCriteriaOccurence(3);
			return structure;
		}


	}while(hit.material.emission == false && bounce <= max);
	return structure;
}


void Engine::bounceRay(HitInfo hit, Ray r, int bounce, bool directLight, std::vector<HitInfo> &structure) {

	if(directLight)
	{
		if(bounce == 0)//direct light shadowray
		{
			hit.r = generateShadowRay(hit.calcIntersectionCoord() + hit.normal*selfIntersectionThreshold);
			structure.push_back(hit);
		}
		else
		{
			if((hit.material.emission == true))//it's a light: stop
			{
				hit.r = Ray(Vector3(0,0,0),hit.calcIntersectionCoord());
				structure.push_back(hit);
			}
			else//shadow
			{
				hit.r = Ray(Vector3(0,0,0),Vector3(0,0,0));
				hit.material = Material(Color(0,0,0),true,0);
				structure.push_back(hit);
			}
		}
	}
	else
	{
		if((hit.material.emission == true && bounce !=1))//it's a light: stop
		{
			hit.r = Ray(Vector3(0,0,0),hit.calcIntersectionCoord());
			structure.push_back(hit);
		}
		else//otherwise, bounce
		{
			hit.r = Ray(importanceSampling(hit.normal),hit.calcIntersectionCoord() + hit.normal*selfIntersectionThreshold);//new bounce ray from the intersection point (+ a small offset to prevent self intersecting artifacts)
			structure.push_back(hit);
		}
	}
	return;
}


//_______________________________
//computeLightAlongRay





//-----------------------------------------------------
//A REECRIRE
//-----------------------------------------------------
//compute pixel color from the multiple data gathered by buildIntersectionStructure(...)
Color Engine::computeLightAlongRay(Ray camRay, HitInfo &cache) {

	//std::vector<HitInfo> directStructure;
	std::vector<HitInfo> directStructure = buildDirectLightStructure(camRay, cache);
	Color outputDirect = Color(0,0,0);

	if(directStructure.size() > 0 && directStructure.at(directStructure.size()-1).r.dir == Vector3(0,0,0))
	{
		for(signed int i =directStructure.size()-1; i>=0; i--)//we travel across the structure backward
		{
			//last ray hits a light
			if(i==directStructure.size()-1)
			{
				outputDirect = directStructure.at(i).material.diffuse*directStructure.at(i).material.emissionPower;
			}

			else//at each bounce, the light intensity is absorbed (material dependent)
			{
				Ray shadowRay = directStructure.at(i).r;
				float Phi = Vector3::calcNorm(shadowRay.dir);
				float invpdf = (2*3.14*(1-std::cos(Phi)));
				Vector3 v = directStructure.at(i).normal.normalize();
				float cosine = abs(Vector3::dot(v,shadowRay.dir.normalize()));//Absorption coef

				outputDirect = outputDirect*(1.0/(3.14))*directStructure.at(i).material.diffuse*(invpdf)*cosine; //cosinus distrib
			}
		}
	}

	std::vector<HitInfo> indirectStructure;
	indirectStructure = buildIndirectLightStructure(camRay, cache);

	Color outputIndirect = Color(0,0,0);
		if(indirectStructure.size() > 0 && indirectStructure.at(indirectStructure.size()-1).r.dir == Vector3(0,0,0))
		{
			for(int i =indirectStructure.size()-1; i>=0; i--)//we travel across the structure backward
			{
				//last ray hits a light
				if(i==indirectStructure.size()-1) outputIndirect = indirectStructure.at(i).material.diffuse*indirectStructure.at(i).material.emissionPower;

				else//at each bounce, the light intensity is absorbed (material dependent)
				{
					outputIndirect = outputIndirect*indirectStructure.at(i).material.diffuse; //cosinus distrib
				}
			}
		}

	return outputDirect*1 +outputIndirect*1;
}

//-------------------------------
//Geometry
//-------------------------------

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

	float u = distribution(generator);//[,1]
	float theta = distribution(generator)*2*3.14;//[0,2pi]


	float Phi = std::asin(std::pow(u,0.5));//distribution: change pow to get various cos^n distrib over the sphere (pow=1 means uniform)
	Vector3 localV = Vector3( std::sin(Phi)*std::cos(theta),std::sin(Phi)*std::sin(theta),std::cos(Phi));

	Vector3 Z = normal;
	Vector3 X;
	Vector3 Y;

	//A REECRIRE
	if(Z.y != 0) Y = Vector3(Z.y,-(Z.x + Z.z),Z.y);
	else  Y = Vector3(Z.z,0,-Z.x);

	X = Vector3::cross(Y,Z);
	Vector3 worldV =  X*localV.x + Y*localV.y + Z*localV.z;

	return worldV;
}

Vector3 Engine::uniformRndInSolidAngle(Vector3 normal, float angle) {

	float u = distribution(generator);
	float theta = distribution(generator)*2*3.14;
	float Phi = std::acos(1-(u*(1-std::cos(angle))));

	Vector3 localV = Vector3( std::sin(Phi)*std::cos(theta),std::sin(Phi)*std::sin(theta),std::cos(Phi) );
	Vector3 Z = normal;
	Vector3 X;
	Vector3 Y;



	//localV = Vector3( std::sin(Phi)*std::cos(theta),std::sin(Phi)*std::sin(theta),std::cos(Phi) );

	//A REECRIRE
	if(Z.y != 0) Y = Vector3(Z.y,-(Z.x + Z.z),Z.y);
	else  Y = Vector3(Z.z,0,-Z.x);

	X = Vector3::cross(Y,Z);
	Vector3 worldV =  X*localV.x + Y*localV.y + Z*localV.z;

	return worldV;
}


float Engine::triangleViewAngle(Triangle t, Vector3 viewerPosition) {

	std::vector<float> result;
	std::vector<Vector3> vertex;
	vertex.push_back(t.a);vertex.push_back(t.b);vertex.push_back(t.c);

	for(int i =0;i<3;i++)
	{
		Vector3 d = (t.calcCenter() - viewerPosition).normalize();
		Vector3 da = (vertex.at(i) - viewerPosition).normalize();
		float dot = Vector3::dot(da,d);

		result.push_back(std::acos(dot));
	}

	return Utility::max(result);

}
//___________________
//generateShadowRay

//Modif pour lumieres multiples;
Ray Engine::generateShadowRay(Vector3 origin) {

	Triangle t;
	float u = distribution(generator);
	if(lightTriangleList.size()!=0)
	{
		if(u>=0)
		{
			t = lightTriangleList.at(0);
		}
		else
		{
			t = lightTriangleList.at(1);
		}
		float Phi = triangleViewAngle(t, origin);
		Vector3 direction = uniformRndInSolidAngle((t.calcCenter() - origin), Phi).normalize()*(Phi);
		//Vector3 direction = (t.calcCenter() - origin).normalize()*Phi;//adhoc shadowRay
		Ray r = Ray(direction,origin);
		return r;
	}
	else
	{
		Ray r = Ray();
		return r;
	}
}


