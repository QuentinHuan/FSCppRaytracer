/*
 * Engine.cpp
 *
 *  Created on: Sep 4, 2019
 *      Author: huan
 */

#include "Engine.hpp"

Engine::Engine(int resX, int resY, Camera &cam, Object obj, std::vector<Triangle> &lightTriangleList, int maxBounce, BVH bvh) : lightTriangleList(lightTriangleList), bvh(bvh), maxBounce(maxBounce)
{
	this->obj = obj;
	TriangleList = obj.faces;
	this->resX = resX;
	this->resY = resY;
	generator = std::default_random_engine();
	distribution = std::uniform_real_distribution<float>(0, 1.0);
	intUniformDistribution = std::uniform_int_distribution<int>(0, lightTriangleList.size() - 1);
	background = obj.getMat(0);
	cache.reserve(resX * resY);
	buildCache(cam);
}

std::shared_ptr<Material> Engine::getMat(int index)
{
	return obj.getMat(index);
}

Ray Engine::shadowRay(HitInfo &hit)
{
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
	return directLight(pixel) + globalIllumination(pixel) * getMat(cache.at(pixel).material)->getBaseColor();
}

Color Engine::directLight(int pixel)
{
	HitInfo hit = cache.at(pixel);
	Color light, final;

	if (hit.hitSomething)
	{
		//not emissive
		if (getMat(hit.material)->type != "emissive")
		{
			Ray shadRay = shadowRay(hit);
			HitInfo shadHit = intersect(shadRay);
			if (shadHit.hitSomething)
			{
				if (getMat(shadHit.material)->type == "emissive")
				{
					light = getMat(shadHit.material)->BRDF(shadHit.r.dir, shadHit.r.dir, shadHit.normal);
					float nDotWi = std::abs(Vector3::dot(hit.normal, shadRay.dir));
					Color BRDF = getMat(hit.material)->BRDF(hit.r.dir * -1, shadRay.dir, hit.normal);

					return light * shadRay.invPDF * BRDF * nDotWi; //direct light, no BRDF sampling
																   //return Color(1, 1, 1) * BRDF;
				}
				else
				{
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
			return getMat(hit.material)->BRDF(Vector3(), Vector3(), Vector3());
		}
	}
	//background
	else
	{
		//statCounter.addCriteriaTime(1,t.elapsed());
		return background->BRDF(Vector3(), Vector3(), Vector3());
	}
}

Color Engine::globalIllumination(int pixel)
{
	HitInfo hit = cache.at(pixel);

	if (hit.hitSomething)
	{
		if (getMat(hit.material)->type == "emissive")
			return Color();
		else //diff
			return GIBounce(hit);
	}
	else
	{
		return Color();
	}
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
			giRay = getMat(hit.material)->sampleBRDF(hit.normal, hit.r.dir, hit.calcIntersectionCoord() + hit.normal * selfIntersectionThreshold, generator, distribution);
			GIHit = intersect(giRay);
		}

		if (GIHit.hitSomething)
		{
			giRay2 = getMat(GIHit.material)->sampleBRDF(GIHit.normal, GIHit.r.dir, GIHit.calcIntersectionCoord() + GIHit.normal * selfIntersectionThreshold, generator, distribution);
			//next Ray
			GIHit2 = intersect(giRay2);
			float nDotWi = std::max(Vector3::dot(GIHit.normal, GIHit2.r.dir), 0.0f);
			if (getMat(GIHit.material)->type == "emissive") //light
			{
				if (bounce != 1)
				{
					Color BRDF = getMat(GIHit.material)->BRDF(GIHit.r.dir, GIHit.r.dir, GIHit.normal);
					final = final * BRDF;
					hitLightSource = true;
				}
				break;
			}
			else //other material
			{
				Color BRDF = getMat(GIHit.material)->BRDF(GIHit.r.dir, GIHit2.r.dir, GIHit.normal);
				//final = final * BRDF * GIHit2.r.invPDF * nDotWi;
				if (!hitLightSource)
				{
					Ray shadRay = shadowRay(GIHit);
					HitInfo shadHit = intersect(shadRay);
					if (shadHit.hitSomething)
					{
						if (getMat(shadHit.material)->type == "emissive")
						{
							nDotWi = std::max(Vector3::dot(GIHit.normal, shadRay.dir), 0.0f);
							final = final * BRDF * getMat(shadHit.material)->BRDF(shadHit.r.dir, shadHit.r.dir, shadHit.normal) * (shadRay.invPDF);
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

				if (bounce == maxBounce)
				{
					final = final * background->BRDF(GIHit.r.dir, GIHit.r.dir, GIHit.normal);
					hitLightSource = true;
					break;
				}
			}
		}
		else //background
		{
			final = final * background->BRDF(GIHit.r.dir, GIHit.r.dir, GIHit.normal);
			hitLightSource = true;
			break;
		}

		hit = GIHit;
		GIHit = GIHit2;
		giRay = giRay2;
	}
	return final;
}

HitInfo Engine::intersect(Ray &r)
{
	Timer t{};
	std::vector<HitInfo> allHit;
	allHit.reserve(TriangleList.size());

	std::vector<Triangle> Tref = std::vector<Triangle>();

	Tref = bvh.testRay(r);

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

	if (allHit.size() > 0)
		return HitInfo::sortForeground(allHit);
	else
		return HitInfo();
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
