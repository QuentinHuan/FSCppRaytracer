/*
 * Node.hpp
 *
 *  Created on: Feb 5, 2020
 *      Author: huan
 */
#pragma once
#ifndef NODE_HPP_
#define NODE_HPP_
#include <vector>
#include <string>
#include "Triangle.hpp"
#include "Box.hpp"

//-----------------------------------------------
//Node class. Parts of the binary tree BVH system
//-----------------------------------------------

class Node {
public:
	Node * childR;//child reference of the node
	Node * childL;
	int primOffset, nPrimitives;//index of the first primitive in the sorted BVH primitives array
	

	Vector3 centroid;
	Box box;
	std::vector<Triangle> infoArray;
	int splitOn =0;

	Node();
	Node(std::vector<Triangle> infoArray);//Interior node creation
	Node(int primOffset,int nPrimitives);//leaf creation
	void split(Node* child1, Node* child2);//split the current node into 2 nodes along one axis (depends on splittingMethod). Initialise the childs of the node with the result
	void computeBoundingBox();
private:

	
};

#endif /* NODE_HPP_ */
