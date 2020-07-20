/*
 * Object.hpp
 *
 *  Created on: Sep 3, 2019
 *      Author: huan
 */
#pragma once
#ifndef OBJECT_HPP_
#define OBJECT_HPP_


#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include "Vector3.hpp"
#include "Utility.hpp"
#include "Triangle.hpp"

class Object {
public:
	std::vector<Triangle> faces;

	Object();
	Object(std::vector<Triangle> triArray);
	Object(std::string file);

	static Object load(std::string filename);
	Vector3 getCenter();
	float getScale();
};

#endif /* OBJECT_HPP_ */
