/*
 * BSP.hpp
 *
 *  Created on: Oct 3, 2019
 *      Author: huan
 */
#pragma once
#ifndef BSP_HPP_
#define BSP_HPP_
#include "Material.hpp"
#include "Ray.hpp"
#include "Box.hpp"
#include "Node.hpp"

class BSP {


private:
	int maxDepth;
	float scale;
	Node BSPTree;
	int counter = 0;
	std::vector<Triangle> allTriangle, result;
	std::vector<Node> nodeList;
	float minDistanceFromOrigin;


	Node* buildBSPTree(Node *n);
	bool intersectBox(Ray r, Box b);
	void getTriangles(Node *n);
	void getTrianglesHitByRay(Node *n, Ray r);
public:

	BSP();
	BSP(int maxDepth, std::vector<Triangle> allTriangle, float scale, Vector3 offset);




	std::vector<Triangle> accelerationStructure(Ray r);
	std::vector<Triangle> debugPartition();
};

#endif /* BSP_HPP_ */
