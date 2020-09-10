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

class Material {
public:

	Color diffuse;
	bool emission;
	float emissionPower;
	float sigma = 0;

	Material();
	Material(Color diffuse);
	Material(Color diffuse,bool emission, float emissionPower);

	float brdf(Vector3 n, Vector3 wr, Vector3 wi);//normal vect, reflection vect and incident vect

};

#endif /* MATERIAL_HPP_ */
