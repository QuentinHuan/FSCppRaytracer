/*
 * BSP.hpp
 *
 *  Created on: Oct 3, 2019
 *      Author: huan
 */
#pragma once
#ifndef BSP_HPP_
#define BSP_HPP_
#include"Node.hpp"
#include "Material.hpp"
#include "Ray.hpp"
#include "Box.hpp"



class BSP {

public:
	int maxDepth;
	float scale;
	Node BSPTree;
	int counter = 0;
	std::vector<Triangle> allTriangle, partition;
	std::vector<Node> nodeList;
	Node* buildBSPTree(Node *n);
	BSP();
	BSP(int maxDepth, std::vector<Triangle> allTriangle, float scale, Vector3 offset);


	std::vector<Triangle> debugPartition();

	bool intersectBox(Ray r, Box b);

	void getTriangles(Node *n);

	std::vector<Triangle> accelerationStructure(Ray r);
	void getTrianglesHitByRay(Node *n, Ray r);

};

#endif /* BSP_HPP_ */
