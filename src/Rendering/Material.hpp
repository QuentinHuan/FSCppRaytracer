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

class Material {
public:

	Color diffuse;
	bool emission;
	float emissionPower;

	Material();
	Material(Color diffuse);
	Material(Color diffuse,bool emission, float emissionPower);
};

#endif /* MATERIAL_HPP_ */
