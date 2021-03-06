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
#include <memory>
#include <string>
#include <fstream>
#include <iostream>
#include "Vector3.hpp"
#include "Utility.hpp"
#include "Triangle.hpp"

class Object
{
public:
	std::vector<Triangle> faces;
	std::vector<std::shared_ptr<Material>> mat;

	Object();
	Object(std::vector<Triangle> triArray);
	Object(std::string file);

	std::shared_ptr<Material> getMat(int index);
	Object load(std::string filename);
};

#endif /* OBJECT_HPP_ */
