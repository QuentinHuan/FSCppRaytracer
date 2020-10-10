/*
 * Engine.cpp
 *
 *  Created on: Sep 4, 2019
 *      Author: huan
 */

#include "Engine.hpp"

Engine::Engine(int resX, int resY, Camera &cam, std::vector<Triangle> &triangleList, std::vector<Triangle> &lightTriangleList, Statistics &statCounter, int maxBounce, BVH bvh) : TriangleList(triangleList), lightTriangleList(lightTriangleList), bvh(bvh), statCounter(statCounter), maxBounce(maxBounce)
{

	this->resX = resX;
	this->resY = resY;
	generator = std::default_random_engine();
	distribution = std::uniform_real_distribution<float>(0, 1.0);
	intUniformDistribution = std::uniform_int_distribution<int>(0, lightTriangleList.size() - 1);
	background = Material(Color(1, 1, 1), true, backgroundPower);
	cache.reserve(resX * resY);
	buildCache(cam);
}

Ray Engine::shadowRay(HitInfo &hit)
{

	Timer t{};
	if (!lightTriangleList.empty())
	{
		int rand = intUniformDistribution(generator);
		Triangle &t = lightTriangleList.at(rand);

		Vector3 origin = hit.calcIntersectionCoord() + hit.normal * selfIntersectionThreshold;
		//Ray r = Ray(t.calcCenter() - origin,origin);//adhoc shadowRay
		Vector3 n = (uniformRndInTriangle(t) - origin);
		float dist = Vector3::calcNorm(n);
		Ray r = Ray(n.normalize(), origin, (lightTriangleList.size()) * (std::max(Vector3::dot(t.normal.normalize(), n.normalize() * (-1)), 0.0f)) * (t.area() / (dist * dist))); //cosine

		return r;
	}
	else
		return Ray();
}

Color Engine::render(int pixel)
{
	//return directLight(pixel);
	//return globalIllumination(pixel);
	return directLight(pixel) + globalIllumination(pixel) * cache.at(pixel).material.diffuse;
}

Color Engine::directLight(int pixel)
{

	HitInfo hit = cache.at(pixel);
	Color light, final;

	if (hit.hitSomething)
	{
		//diffuse
		if (hit.material.emission == false)
		{

			Ray shadRay = shadowRay(hit);
			HitInfo shadHit = intersect(shadRay);
			if (shadHit.hitSomething)
			{
				if (shadHit.material.emission)
				{
					light = shadHit.material.diffuse * shadHit.material.emissionPower;
					float nDotWi = std::abs(Vector3::dot(hit.normal, shadRay.dir));
					Color BRDF = hit.material.BRDF(hit.r.dir * -1, shadRay.dir, hit.normal);

					return light * shadRay.invPDF * BRDF * nDotWi; //direct light, no BRDF sampling
																   //return Color(1, 1, 1) * BRDF;
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
			return hit.material.diffuse * hit.material.emissionPower;
		}
	}
	//background
	else
	{
		//statCounter.addCriteriaTime(1,t.elapsed());
		return background.diffuse * backgroundPower;
	}
}

Color Engine::globalIllumination(int pixel)
{
	HitInfo hit = cache.at(pixel);

	if (hit.hitSomething)
	{
		if (hit.material.emission)
		{
			return Color();
		}
		else //diff
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

Color Engine::GIBounce(HitInfo &hit)
{
	Color light = Color(), final = Color(1, 1, 1);
	bool hitLightSource = false;

	Ray giRay2;
	Ray giRay;
	HitInfo GIHit;
	HitInfo GIHit2;
	for (int bounce = 1; bounce <= maxBounce; bounce++)
	{
		if (bounce == 1)
		{
			giRay = hit.material.sampleBRDF(hit.normal, hit.r.dir, hit.calcIntersectionCoord() + hit.normal * selfIntersectionThreshold, generator, distribution);
			GIHit = intersect(giRay);
		}

		if (GIHit.hitSomething)
		{
			giRay2 = GIHit.material.sampleBRDF(GIHit.normal, GIHit.r.dir, GIHit.calcIntersectionCoord() + GIHit.normal * selfIntersectionThreshold, generator, distribution);
			//next Ray
			GIHit2 = intersect(giRay2);
			float nDotWi = std::max(Vector3::dot(GIHit.normal, GIHit2.r.dir), 0.0f);
			if (GIHit.material.emission) //light
			{
				if (bounce != 1)
				{
					Color BRDF = GIHit.material.diffuse * GIHit.material.emissionPower;
					final = final * BRDF;
					hitLightSource = true;
				}
				break;
			}
			else //other material
			{
				Color BRDF = GIHit.material.BRDF(GIHit.r.dir, GIHit2.r.dir, GIHit.normal);
				final = final * BRDF * GIHit2.r.invPDF * nDotWi;
				if (bounce == maxBounce)
				{
					final = final * background.diffuse * background.emissionPower;
					hitLightSource = true;
					break;
				}
			}
		}
		else //background
		{
			final = final * background.diffuse * background.emissionPower;
			hitLightSource = true;
			break;
		}

		if (!hitLightSource)
		{
			Ray shadRay = shadowRay(GIHit);
			HitInfo shadHit = intersect(shadRay);
			if (shadHit.hitSomething)
			{
				if (shadHit.material.emission)
				{
					final = final * shadHit.material.diffuse * shadHit.material.emissionPower * (shadRay.invPDF);
					hitLightSource = true;
					break;
				}
				else
				{
					if (bounce == maxBounce)
						return Color(0, 0, 0);
				}
			}
			else
			{
				if (bounce == maxBounce)
					return Color(0, 0, 0);
			}
		}

		hit = GIHit;
		GIHit = GIHit2;
		giRay = giRay2;
	}
	return final;
}

//doesn't works, rotation of the generated vector is incorrect
Ray Engine::cosineWeightedInSolidAngle(float angle, Vector3 direction, Vector3 position)
{
	float u = distribution(generator);				  //[,1]
	float theta = distribution(generator) * 2 * 3.14; //[0,2pi]

	float Phi = std::acos(1 - (u * (1 - std::cos(angle))));
	Vector3 localV = Vector3(std::sin(Phi) * std::cos(theta), std::sin(Phi) * std::sin(theta), std::cos(Phi));

	Vector3 axis = Vector3::cross(Vector3(0, 0, 1), direction).normalize();
	if (axis == Vector3())
		return Ray(localV * direction.z, position, angle);
	else
	{
		float rotAngle = std::acos(Vector3::dot(direction.normalize(), Vector3(0, 0, 1)));

		Vector3 w = Quaternion::rotate(rotAngle, axis, localV);

		return Ray(w, position, angle);
	}
}

Ray Engine::uniformInHemisphere(Vector3 direction, Vector3 position)
{
	Vector3 v = Vector3(distribution(generator), distribution(generator), distribution(generator));
}

HitInfo Engine::intersect(Ray &r)
{
	Timer t{};
	std::vector<HitInfo> allHit;
	allHit.reserve(TriangleList.size());

	std::vector<Triangle> Tref = std::vector<Triangle>();

	if (useAccelerationStructure)
	{
		Tref = bvh.testRay(r);
	}
	else
	{
		Tref = TriangleList;
	}

	statCounter.addCriteriaTime(1, t.elapsed());

	//fill the vector with the coord of all intersection points (in ray space)
	for (auto it = Tref.begin(); it != Tref.end(); it++)
	{
		HitInfo h{};
		h = it->intersect(r);
		if (h.hitSomething)
		{
			allHit.push_back(h);
		}
	}

	if (debugBVH)
	{ /*
		//std::vector<Box> B = bvh.testRayDEBUG(r,8);
		//fill the vector with the coord of all intersection points (in ray space)
		for(auto it = B.begin(); it != B.end(); it++)
		{
			HitInfo h{};
			h = it->intersectDebug(r);
			if(h.hitSomething)
			{
				allHit.push_back(h);
			}
		}*/
	}

	if (allHit.size() > 0)
	{
		return HitInfo::sortForeground(allHit);
	}
	else
	{
		return HitInfo();
	}
}

Vector3 Engine::uniformRndInTriangle(Triangle t)
{

	float u1 = distribution(generator);

	float u2 = distribution(generator);

	float sqrt = std::pow(u1, 0.5);

	float x = 1 - sqrt;

	float y = u2 * sqrt;

	Vector3 U = t.b - t.a;

	Vector3 V = t.c - t.a;

	Vector3 v = t.a + (U * x + V * y);

	return v;
}

void Engine::buildCache(Camera &cam)
{

	int counter = 0;
	for (int i = 0; i < resX; i++)
	{
		for (int j = 0; j < resY; j++)
		{
			Ray r = cam.camRay(i, j);
			cache.push_back(intersect(r));
		}

		if (i == 0)
		{
			std::cout << "[";
		}
		//cout  << (100.0*i)/(float)resX;
		if (((100.0 * i) / (float)resX) > 5 * counter)
		{
			std::cout << "|";
			std::cout.flush();
			counter++;
		}
		if (i == resX - 1)
		{
			std::cout << "]" << std::endl;
		}
	}

	std::cout << "Cache Building done" << std::endl;
}
