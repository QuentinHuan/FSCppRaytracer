/*
 * Material.cpp
 *
 *  Created on: Sep 14, 2019
 *      Author: huan
 */

#include "Material.hpp"

Material::Material() : diffuse(Color()), emission(false), emissionPower(0){}

Material::Material(Color diffuse) : diffuse(diffuse), emission(false), emissionPower(0) {}

Material::Material(Color diffuse, bool emission, float emissionPower) : diffuse(diffuse), emission(emission), emissionPower(emissionPower){}
