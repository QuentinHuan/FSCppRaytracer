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

	BVH(std::vector<Triangle> triangleList);
	void build(std::vector<Node*> Nodes);

	std::vector<Node*> kNearestNeighbours(std::vector<Node*> Nodes, Node * refNode,int k);

};

#endif /* BVH_HPP_ */
