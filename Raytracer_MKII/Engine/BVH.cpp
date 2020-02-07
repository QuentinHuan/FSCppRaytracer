/*
 * BVH.cpp
 *
 *  Created on: Feb 5, 2020
 *      Author: huan
 */

#include <BVH.hpp>

BVH::BVH(std::vector<Triangle> triangleList) {
	this->triangleList = triangleList;
}

void BVH::build(std::vector<Node*> Nodes) {

	if(Nodes.size() == 1)
	{
		tree = Nodes.at(0);
		return;
	}
	else
	{
		//NodeList.push_back();
	}
}

std::vector<Node*> BVH::trierNode(std::vector<Node*> Nodes, Node* refNode) {
	std::vector<Node*> triee = std::vector<Node*>();

	for (int i = 0; i < Nodes.size(); i++) {


	}
}
