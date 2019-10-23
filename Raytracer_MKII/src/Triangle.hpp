/*
 * Triangle.hpp
 *
 *  Created on: Sep 1, 2019
 *      Author: huan
 */
#pragma once
#ifndef TRIANGLE_HPP_
#define TRIANGLE_HPP_
#include "Material.hpp"
#include "Vector3.hpp"


class Triangle {
public:
	Vector3 a,b,c,normal;
	Material material;
	Triangle();
	Triangle(Vector3 a,Vector3 b,Vector3 c, Material material);
	Triangle(Vector3 a,Vector3 b,Vector3 c,Vector3 normal, Material material);

	Vector3 calcNormal();
	Vector3 calcCenter();
};

#endif /* TRIANGLE_HPP_ */
