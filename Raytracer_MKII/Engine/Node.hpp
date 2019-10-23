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

private:
	Vector3 planeOrigin,planeNormal, treeCenter;
	bool direct;
	float scale;

	int getDepth();
	void addRChild(Node *n);
	void addLChild(Node *n);
	void addPlane(bool direct);
	void addBox(bool direct);
	int isTriangleInside(Triangle t);
	bool isVector3Inside(Vector3 t);

public:

	Node();
	Node(Node *father, bool leaf, bool origin, bool direct, float scale);
	Node(std::vector<Triangle> list, Node *father, bool leaf, bool origin, bool direct, float scale, Vector3 center);

	int depth;
	Node *rChild,*lChild;
	Node *father;
	bool leaf, origin;
	Box box;

	std::vector<Triangle> triangles;

	void sort();









};

#endif /* NODE_HPP_ */
