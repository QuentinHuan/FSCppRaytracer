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
Engine::Engine(std::vector<Object> objectList, Statistics &statCounter,BSP &bsp,int maxBounce,bool useAccelerationStructure) {
	this->objectList = objectList;
	generator = std::default_random_engine();
	distribution = std::uniform_real_distribution<float>(-1.0,1.0);
	this->statCounter = &statCounter;

	this->heuristic = heuristic;
	this->maxBounce = maxBounce;

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

//MAIN
Color Engine::rayTrace(Ray camRay,int pixelIndex) {
	Color c = computeLightAlongRay(camRay,pixelIndex);

	if(useCache)
	{

	}

	return c;
}


//-------------------------------
//Ray tracing Logic
//-------------------------------

//_______________________________
//buildIntersectionStructure

//compute all the intersections from camera to the first light hit, bouncing off objects
std::vector<HitInfo> Engine::buildIntersectionStructure(Ray camRay,int pixelIndex) {
	std::vector<HitInfo> structure;
	HitInfo hit{};
	int bounce = 0;

	//fire bounce rays until it reaches a light source
	statCounter->addCriteriaOccurence(0);//number of rays from the camera
	do
	{
		Ray r;
		if(structure.size() == 0)
		{
			if(useCache)
			{
				structure.push_back(camRayCache.at(pixelIndex));
				continue;
			}
			else
			{
				r = camRay; //first ray is the ray from the camera
			}
		}

		else r = structure.at(structure.size()-1).r;//otherwise, we cast a ray starting from the intersection, in the direction computed during the last iteration

		if(rayCast(r,hit))//hit something:
		{
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
					hit.r = Ray(randDirection(hit.normal,90),hit.calcIntersectionCoord() + hit.normal*selfIntersectionThreshold);//new bounce ray from the intersection point (+ a small offset to prevent self intersecting artifacts)
					structure.push_back(hit);
				}
			}
			bounce++;
		}
		else//hit nothing, stop
		{
			hit.r = Ray(Vector3(0,0,0),Vector3(0,0,0));
			hit.material = Material(Color(1,1,1),true,1);
			structure.push_back(hit);
			statCounter->addCriteriaOccurence(3);
			return structure;
		}
	}while(hit.material.emission == false && bounce <= maxBounce);
	return structure;
}

//_______________________________
//computeLightAlongRay

//compute pixel color from the multiple data gathered by buildIntersectionStructure(...)
Color Engine::computeLightAlongRay(Ray camRay,int pixelX,int pixelY) {
	//std::vector<HitInfo> structure = buildIntersectionStructure(camRay);
	std::vector<HitInfo> structure = buildIntersectionStructure(camRay,pixelX,pixelY);

	Color output = Color(0,0,0);
	if(structure.size() > 0 && structure.at(structure.size()-1).r.dir == Vector3(0,0,0))
	{
		for(int i =structure.size()-1; i>=0; i--)//we travel across the structure backward
		{
			//last ray hit a light
			if(i==structure.size()-1) output = structure.at(i).material.diffuse * structure.at(i).material.emissionPower;

			else//at each bounce, the light intensity is absorbed (material dependent)
			{
				Vector3 v = structure.at(i).normal.normalize();
				float lambert = Vector3::dot(v,structure.at(i).r.dir.normalize());//Absorption coef
				output = output*structure.at(i).material.diffuse*abs(lambert) *2.0;
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
bool Engine::rayCast(Ray r, HitInfo &hitInfo) {

	bool bHit = false;
	std::vector<HitInfo> allHit;



	for(Object o : objectList)
	{

		HitInfo res(r,0,Material(),Vector3(0,0,0));
		bool ressult = intersectObject(o,r,res);

		if(ressult)
		{
			allHit.push_back(res);
			bHit = true;
		}
	}

	if(bHit)
	{
		//Timer t{};
		hitInfo = HitInfo::sortForeground(allHit);
		//statCounter->addCriteriaTime(1,t.elapsed());
		return true;
	}
	else
	{

		return false;
	}
}

//PRIVATE:----------------------------

//___________________
//intersectObject

//perform a ray cast against one object
//return true if the ray hit something
//all useful info (coord, material ect...) are stored in the HitInfo object provided
bool Engine::intersectObject(Object obj, Ray r, HitInfo &hitInfo) {
	bool result = false;
	std::vector<HitInfo> allHit;

	if(useAccelerationStructure)
	{
		obj.faces = accelerationStructure.accelerationStructure(r);
	}


	//fill the vector with the coord of all intersection points (in ray space)
	for(auto tri : obj.faces)
	{
		HitInfo h{};
		bool hit = intersectTri(tri,r,h);
		if(hit)
		{
			result = true;
			allHit.push_back(h);
		}
	}
	if(result == true)
	{
		hitInfo = HitInfo::sortForeground(allHit);
		return true;
	}
	else
	{
		return false;
	}
}

//___________________
//randDirection

Vector3 Engine::randDirection(Vector3 normal, float angle) {

	float r1 = distribution(generator);
	float r2 = distribution(generator);
	float r3 = distribution(generator);
	Vector3 v = Vector3(r1,r2,r3).normalize();

	float deg2Rad = 3.14/180.0;

	//float currentAngle = std::atan2( Vector3::calcNorm(Vector3::cross(v,normal)), Vector3::dot(v,normal))*180/3.14;
	float currentAngle = Vector3::dot(normal,v);
	while(currentAngle < 0)
	{

		float r1 = distribution(generator);
		float r2 = distribution(generator);
		float r3 = distribution(generator);
		v = Vector3(r1,r2,r3).normalize();
		//currentAngle = std::atan2( Vector3::calcNorm(Vector3::cross(v,normal)), Vector3::dot(v,normal))*180/3.14;
		currentAngle = Vector3::dot(normal,v);
	}
	return v;
}

//___________________
//intersectTri

//utility fonction, compute the intersection point of a ray and with a tri
//output is the coord of the intersection point in ray space
bool Engine::intersectTri(Triangle tri, Ray r, HitInfo &hitInfo) {

	//Timer t {}; // statistics

	hitInfo.r = r;
	hitInfo.intersection = 0;
	hitInfo.material = tri.material;

	Vector3 u,v,k,n,d;
	u = tri.b - tri.a;
	v = tri.c - tri.a;
	k = r.dir;
	n=Vector3::cross(u,v);
	hitInfo.normal = tri.normal;

	d = r.pos - tri.a;

	Vector3 result(0,0,0);//resulting intersection, in base (u,v,k)
	float denominator = Vector3::dot(n,k);
	if(denominator != 0)
	{
		float invDen = 1/denominator;
		result.x = Vector3::dot(Vector3::cross(d,v),k)*invDen;
		result.y = Vector3::dot(Vector3::cross(u,d),k)*invDen;
		result.z = -Vector3::dot(n,d)*invDen;

		if(0<=result.x && result.x<=1 && 0<=result.y && result.y<=1 && result.z >= 0 && (result.x + result.y) <= 1)
		{
			hitInfo.intersection = result.z;
			//statCounter->addCriteriaTime(0,t.elapsed());
			return true;
		}
		//not in the triangle
		else
		{
			//statCounter->addCriteriaTime(0,t.elapsed());
			return false;
		}
	}
	//don't intersect with plane
	else
	{
		//statCounter->addCriteriaTime(0,t.elapsed());
		return false;
	}
}

//___________________
//generateShadowRay

//A modifier pour plusieur lumière;
Ray Engine::generateShadowRay(Vector3 origin) {

	//Vector3 n = randDirection(Vector3(1,0,0),360);
	Vector3 direction = (lightTriangleList.at(0) - origin).normalize();
	Ray r = Ray(randDirection(direction,10),origin);
	return r;
}

//___________________
//intersectTriMoller

bool Engine::intersectTriMoller(Triangle tri, Ray r, HitInfo &hitInfo) {

	hitInfo.r = r;
	hitInfo.intersection = 0;
	hitInfo.material = tri.material;
	hitInfo.normal = tri.normal;

	Vector3 u,v;
	u = tri.b - tri.a;
	v = tri.c - tri.a;

	Vector3 pvec = Vector3::cross(r.dir,v);
	float det = Vector3::dot(u,pvec);

	// if the determinant is negative the triangle is backfacing
	// if the determinant is close to 0, the ray misses the triangle
	if (det < kEpsilon) return false;
	// ray and triangle are parallel if det is close to 0
	if (fabs(det) < kEpsilon) return false;


	float invDet = 1.0 / det;
	Vector3 tvec = r.pos - tri.a;
	float f = Vector3::dot(tvec,pvec) * invDet;

	if (f < 0 || f > 1) return false;

	Vector3 qvec = Vector3::cross(tvec,u);
	float g = Vector3::dot(r.dir,qvec) * invDet;
	if (g < 0 || f + g > 1) return false;

	float h = Vector3::dot(v,qvec) * invDet;

	hitInfo.intersection = h;

	return true;
}
