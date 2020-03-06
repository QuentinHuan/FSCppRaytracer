/*
 * Engine.cpp
 *
 *  Created on: Sep 4, 2019
 *      Author: huan
 */

#include "Engine.hpp"



Engine::Engine(int resX,int resY,Camera &cam,std::vector<Triangle> & triangleList,std::vector<Triangle> & lightTriangleList, Statistics &statCounter,int maxBounce, BVH bvh) : TriangleList(triangleList),lightTriangleList(lightTriangleList), bvh(bvh),statCounter(statCounter),maxBounce(maxBounce) {

		this->resX = resX;
		this->resY = resY;
		generator = std::default_random_engine();
		distribution = std::uniform_real_distribution<float>(0,1.0);
		intUniformDistribution = std::uniform_int_distribution<int>(0, lightTriangleList.size()-1);
		background = Material(Color(1,1,1), true, backgroundPower);
		cache.reserve(resX*resY);
		buildCache(cam);
}


Ray Engine::shadowRay(HitInfo &hit) {

	Timer t{};
		if(!lightTriangleList.empty())
		{
			int rand = intUniformDistribution(generator);
			Triangle &t = lightTriangleList.at(rand);

			Vector3 origin = hit.calcIntersectionCoord() + hit.normal*selfIntersectionThreshold;
			//Ray r = Ray(t.calcCenter() - origin,origin);//adhoc shadowRay
			Vector3 n = (uniformRndInTriangle(t) - origin);
			float dist = Vector3::calcNorm(n);
			Ray r = Ray(n.normalize(),origin, (t.area()/(dist*dist)) );//cosine

			return r;
		}
		else return Ray();
}

Color Engine::render(int pixel) {

	//return directLight(pixel)*cache.at(pixel).material.diffuse;
	//return globalIllumination(camRay,cache);
	return (directLight(pixel) + globalIllumination(pixel))*cache.at(pixel).material.diffuse;
}

Color Engine::directLight(int pixel) {

	HitInfo hit = cache.at(pixel);
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
					return light * (1/3.14)*std::abs(Vector3::dot(hit.normal,shadRay.dir))*shadRay.distributionSolidAngle;
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

Color Engine::globalIllumination(int pixel) {
	HitInfo hit = cache.at(pixel);


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

	Color light = Color(), final = Color(1,1,1);
	bool hitLightSource = false;

	float invPDF = (3.14);//cosine

	for(int bounce = 1; bounce <= maxBounce;bounce++)
	{
		Ray giRay;
		giRay = GIRay(hit);
		HitInfo GIHit = intersect(giRay);

		//float invPDF = 1.0;
		if(GIHit.hitSomething)
		{
			if(GIHit.material.emission)
			{

				if(bounce == 1)
				{
					return Color();
				}
				else
				{
					final = final * GIHit.material.diffuse*GIHit.material.emissionPower*(1.0/3.14)*invPDF;
					hitLightSource = true;
					break;
				}
			}
			else
			{
				final = final * GIHit.material.diffuse*(1.0/3.14)*invPDF;
			}
		}
		else//background
		{
			final =  final * background.diffuse*background.emissionPower*(1.0/3.14)*invPDF;
			hitLightSource = true;
			break;
		}


		Ray shadRay = shadowRay(GIHit);
		HitInfo shadHit= intersect(shadRay);
		if(shadHit.hitSomething)
		{
			if(shadHit.material.emission)
			{
						final = final * shadHit.material.diffuse*shadHit.material.emissionPower*std::abs(Vector3::dot(shadHit.normal,shadRay.dir))*(shadRay.distributionSolidAngle)*(1/3.14)*invPDF;
						hitLightSource = true;
						break;
			}
			else
			{
				if(bounce==maxBounce) return Color(0,0,0);
			}
		}
		else
		{
			if(bounce==maxBounce) return Color(0,0,0);
		}

		hit = GIHit;
	}
	return final;
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
	if(axis == Vector3()) return Ray(localV*direction.z,position,angle);
	else
	{
		float rotAngle = std::acos(Vector3::dot(direction.normalize(),Vector3(0,0,1)));

			Vector3 w = Quaternion::rotate(rotAngle,axis,localV);

			return Ray(w,position,angle);
	}
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


Vector3 Engine::uniformRndInTriangle(Triangle t) {

	float u1 = distribution(generator);

	float u2 = distribution(generator);

	float sqrt = std::pow(u1,0.5);

	float x = 1- sqrt;

	float y = u2*sqrt;



	Vector3 U = t.b-t.a;

	Vector3 V = t.c-t.a;



	Vector3 v = t.a + (U*x + V*y);

	return v;

}


void Engine::buildCache(Camera &cam) {


		int counter = 0;
		for(int i=0;i<resX;i++)
		{
			for(int j=0;j<resY;j++)
			{
				Ray r = cam.camRay(i,j);
				cache.push_back(intersect(r));
			}

			if(i==0)
			{
				std::cout  << "[";
			}
			//cout  << (100.0*i)/(float)resX;
			if(((100.0*i)/(float)resX) > 5*counter)
			{
				std::cout <<"|";
				std::cout.flush();
				counter++;
			}
			if(i==resX-1)
			{
				std::cout <<"]" << std::endl;
			}
		}

		std::cout  <<"Cache Building done" << std::endl;
}
