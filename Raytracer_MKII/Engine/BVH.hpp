/*
 * BVH.hpp
 *
 *  Created on: Feb 5, 2020
 *      Author: huan
 */

#pragma once
#ifndef BVH_HPP_
#define BVH_HPP_
#include "Node.hpp"
#include <list>

class BVH {
public:
	std::vector<Triangle> triangleList;

	Node* tree;
	std::list<Node> NodeList;

	BVH();
	BVH(std::vector<Triangle> triangleList);
	void build(std::vector<Node*> Nodes);

	std::vector<Node*> kNearestNeighbours(std::vector<Node*> Nodes, Node * refNode,int k);

	std::vector<Triangle> testRay(Ray &r);
	std::vector<Box> testRayDEBUG(Ray r,int depthLim);
	int numberOfNode(Node * n, int numb);
private:
	void testRay(Ray &r, Node *n,std::vector<Triangle> * result);
	std::vector<Box> testRayDEBUG(Ray r,Node *n,std::vector<Box> result,int depth,int depthLim);

};

#endif /* BVH_HPP_ */
