/*
 * Engine.cpp
 *
 *  Created on: Sep 4, 2019
 *      Author: huan
 */

#include "Engine.hpp"



Engine::Engine(std::vector<Triangle> & triangleList,std::vector<Triangle> & lightTriangleList, Statistics &statCounter,int maxBounce, BVH bvh) : TriangleList(triangleList),lightTriangleList(lightTriangleList), bvh(bvh),statCounter(statCounter),maxBounce(maxBounce) {

		generator = std::default_random_engine();
		distribution = std::uniform_real_distribution<float>(0,1.0);
		background = Material(Color(1,1,1), true, backgroundPower);
}


Ray Engine::shadowRay(HitInfo &hit) {

	Timer t{};
		if(!lightTriangleList.empty())
		{
			Triangle &t = lightTriangleList.at(0);

			Vector3 origin = hit.calcIntersectionCoord() + hit.normal*selfIntersectionThreshold;
			//Ray r = Ray(t.calcCenter() - origin,origin);//adhoc shadowRay
			Ray r = uniformInSolidAngle(triangleViewAngle(t,origin),t.calcCenter() - origin,origin);//cosine
			return r;
		}
		else return Ray();
}

Color Engine::rayTrace(Ray &camRay, HitInfo &cache) {

	//return directLight(camRay,cache);
	return globalIllumination(camRay,cache);
	//return directLight(camRay,cache) +  globalIllumination(camRay,cache);
}

Color Engine::directLight(Ray &r, HitInfo &cache) {

	HitInfo hit = cache;
	Color light, final;

	if(hit.hitSomething)
	{
		//diffuse
		if(hit.material.emission == false)
		{

			Ray shadRay = shadowRay(hit);

			HitInfo shadHit = intersect(shadRay);

			if(shadHit.hitSomething)
			{
				if(shadHit.material.emission)
				{
					light = shadHit.material.diffuse*shadHit.material.emissionPower;
					//return hit.material.diffuse*Vector3::dot(hit.normal,shadRay.dir)*(1/3.14)*light;//adhoc
					return hit.material.diffuse*(1/3.14)*light*Vector3::dot(hit.normal,shadRay.dir);
				}
				else
				{
					//statCounter.addCriteriaTime(1,t.elapsed());
					return Color();
				}
			}
			else
			{
				return Color();
			}
		}
		//emissive
		else
		{
			//statCounter.addCriteriaTime(1,t.elapsed());
			return hit.material.diffuse;
		}
	}
	//background
	else
	{
		//statCounter.addCriteriaTime(1,t.elapsed());
		return background.diffuse*background.emissionPower;
	}
}

Color Engine::globalIllumination(Ray &r, HitInfo &cache) {
	HitInfo hit = cache;


	if(hit.hitSomething)
	{
		if(hit.material.emission)
		{
			return Color();
		}
		else//diffuse
		{

			return GIBounce(hit);

		}
	}
	else
	{
		return Color();
	}

	return Color();
}

Color Engine::GIBounce(HitInfo &hit) {

	Color light = Color(), final = hit.material.diffuse;

	for(int bounce = 1; bounce <= maxBounce;bounce++)
	{
		Ray giRay = GIRay(hit);
		HitInfo GIHit = intersect(giRay);

		float Phi = giRay.distributionSolidAngle;
		float invPDF = (3.14*std::pow(std::sin(Phi),2));//cosine

		//float invPDF = 1.0;


		if(GIHit.hitSomething)
		{
			if(GIHit.material.emission)
			{
				if(hit.material.diffuse.g == 1 && hit.material.diffuse.r == 0 && hit.material.diffuse.b == 0 )
				{
					int a =2;
				}
				light = GIHit.material.diffuse*GIHit.material.emissionPower;
				break;
			}
			else
			{
				final = final * GIHit.material.diffuse*(1.0/3.14)*invPDF;
			}
		}
		else//background
		{
			light = background.diffuse*background.emissionPower;
		}
		hit = GIHit;
	}

	return final * light;
}

Ray Engine::GIRay(HitInfo &hit) {

	return cosineWeightedInSolidAngle(3.14/2,hit.normal,hit.calcIntersectionCoord()+hit.normal*selfIntersectionThreshold);
}

//doesn't works, rotation of the generated vector is incorrect
Ray Engine::cosineWeightedInSolidAngle(float angle,Vector3 direction, Vector3 position)
{
	float u = distribution(generator);//[,1]
	float theta = distribution(generator)*2*3.14;//[0,2pi]


	float Phi = std::acos(1-(u*(1-std::cos(angle))));//distribution: change pow to get various cos^n distrib over the sphere (pow=1 means uniform)
	Vector3 localV = Vector3( std::sin(Phi)*std::cos(theta),std::sin(Phi)*std::sin(theta),std::cos(Phi));

	Vector3 axis = Vector3::cross(Vector3(0,0,1),direction).normalize();
	float rotAngle = std::acos(Vector3::dot(direction.normalize(),Vector3(0,0,1)));

	Vector3 w = Quaternion::rotate(rotAngle,axis,localV);

	return Ray(w,position,angle);
}

Ray Engine::uniformInSolidAngle(float angle,Vector3 direction, Vector3 position)
{
	float u = distribution(generator);//[,1]
	float theta = distribution(generator)*2*3.14;//[0,2pi]


	float Phi = u*angle;//distribution: change pow to get various cos^n distrib over the sphere (pow=1 means uniform)
	Vector3 localV = Vector3( std::sin(Phi)*std::cos(theta),std::sin(Phi)*std::sin(theta),std::cos(Phi));

	Vector3 axis = Vector3::cross(Vector3(0,0,1),direction.normalize());
	float rotAngle = std::acos(Vector3::dot(direction.normalize(),Vector3(0,0,1)));




	//Vector3 worldV =  Vector3(X.x * localV.x + Y.x * localV.y + Z.x * localV.z, X.y * localV.x + Y.y * localV.y + Z.y * localV.z, X.z * localV.x + Y.z * localV.y + Z.z * localV.z );
	Vector3 w = Quaternion::rotate(rotAngle,axis,localV);

	return Ray(w,position,angle);
}





HitInfo Engine::intersect(Ray &r) {
	Timer t{};
	std::vector<HitInfo> allHit;
	allHit.reserve(TriangleList.size());

	std::vector<Triangle> Tref = std::vector<Triangle>();

	Tref = bvh.testRay(r);
	statCounter.addCriteriaTime(1,t.elapsed());

	//fill the vector with the coord of all intersection points (in ray space)
	for(auto it = Tref.begin(); it != Tref.end(); it++)
	{
		HitInfo h{};
		h = it->intersect(r);
		if(h.hitSomething)
		{
			allHit.push_back(h);
		}
	}

	if(debugBVH)
	{
		std::vector<Box> B = bvh.testRayDEBUG(r,8);
		//fill the vector with the coord of all intersection points (in ray space)
		for(auto it = B.begin(); it != B.end(); it++)
		{
			HitInfo h{};
			h = it->intersectDebug(r);
			if(h.hitSomething)
			{
				allHit.push_back(h);
			}
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


HitInfo Engine::buildCache(Ray &r) {

	return intersect(r);
}
