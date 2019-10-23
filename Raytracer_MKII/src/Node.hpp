/*
 * Node.hpp
 *
 *  Created on: Oct 3, 2019
 *      Author: huan
 */

#pragma once
#ifndef NODE_HPP_
#define NODE_HPP_
#include "Triangle.hpp"
#include <vector>
#include "Box.hpp"


class Node {

public:

	Node();
	Node(Node *father, bool leaf, bool origin, bool direct, float scale);
	Node(std::vector<Triangle> list, Node *father, bool leaf, bool origin, bool direct, float scale, Vector3 center);

	int depth;



	Node *rChild,*lChild;
	Node *father;
	bool leaf, origin;
	Vector3 planeOrigin,planeNormal, treeCenter;
	Box box;

	std::vector<Triangle> triangles;
	void sort();
	int isTriangleInside(Triangle t);
	bool isVector3Inside(Vector3 t);

	void addRChild(Node *n);
	void addLChild(Node *n);
	void addPlane(bool direct);
	void addBox(bool direct);
	float scale;
	bool direct;
	int getDepth();



};

#endif /* NODE_HPP_ */
