/*
 * Triangle.hpp
 *
 *  Created on: Sep 1, 2019
 *      Author: huan
 */
#pragma once
#ifndef TRIANGLE_HPP_
#define TRIANGLE_HPP_
#include "Vector3.hpp"
#include "HitInfo.hpp"
#include "Box.hpp"

class Triangle
{
public:
	Vector3 a, b, c, normal;
	int material;
	Box bBox;
	Triangle();
	Triangle(Vector3 a, Vector3 b, Vector3 c, int material);
	Triangle(Vector3 a, Vector3 b, Vector3 c, Vector3 normal, int material);

	HitInfo intersect(Ray &r);
	HitInfo intersectMoller(Ray &r);

	Vector3 calcNormal();
	Vector3 calcCenter();

	float area();

	static Box boundingBox(std::vector<Triangle> T);
};

#endif /* TRIANGLE_HPP_ */
