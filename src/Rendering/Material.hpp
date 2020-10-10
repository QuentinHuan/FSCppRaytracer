/*
 * Material.hpp
 *
 *  Created on: Sep 14, 2019
 *      Author: huan
 */
#pragma once
#ifndef MATERIAL_HPP_
#define MATERIAL_HPP_
#include "Color.hpp"
#include "Vector3.hpp"
#include "Quaternion.hpp"
#include "Ray.hpp"
#include <random>

class Material
{
public:
	Color diffuse;
	bool emission;
	float emissionPower;
	float alpha;

	Material();
	Material(Color diffuse, float alpha);
	Material(Color diffuse, bool emission, float emissionPower);

	Color BRDF(Vector3 v, Vector3 l, Vector3 n);
	Ray sampleBRDF(Vector3 n, Vector3 v, Vector3 position, std::default_random_engine &generator, std::uniform_real_distribution<float> &distribution);
};

#endif /* MATERIAL_HPP_ */
