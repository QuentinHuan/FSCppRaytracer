/*
 * Material.cpp
 *
 *  Created on: Sep 14, 2019
 *      Author: huan
 */

#include "Material.hpp"

Material::Material() {
	// TODO Auto-generated constructor stub
	diffuse = Color(0,0,0);
	emission = false;
	emissionPower = 0;
}

Material::Material(Color diffuse) {
	// TODO Auto-generated constructor stub
	this->diffuse = diffuse;
	emission = false;
	emissionPower = 0;
}

Material::Material(Color diffuse, bool emission, float emissionPower) {
	this->diffuse = diffuse;
	this->emission = emission;
	this->emissionPower = emissionPower;
}
