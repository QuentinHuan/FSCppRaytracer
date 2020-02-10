/*
 * BVH.cpp
 *
 *  Created on: Feb 5, 2020
 *      Author: huan
 */

#include <BVH.hpp>

BVH::BVH(std::vector<Triangle> triangleList) : triangleList(triangleList) {

	std::vector<Node*> newSet;
	NodeList = std::list<Node>();
	tree = 0;
	for (int i = 0; i < triangleList.size(); i++) {

		NodeList.push_back(Node(0,triangleList.at(i)));
		newSet.push_back(&NodeList.back());
	}


	build(newSet);
}

bool contain(std::vector<Node*> A, Node* B)
{

	for (int i = 0; i < A.size(); i++) {
		if(A.at(i) == B)
		{
			return true;
		}
	}
	return false;
}

void BVH::build(std::vector<Node*> Nodes) {

	if(Nodes.size() == 1)
	{
		tree = Nodes.at(0);
		return;
	}
	else
	{
		std::vector<Node*> newSet = std::vector<Node*>();
		int it =0;
		while(Nodes.size() != 0)
		{
			std::vector<Node*> fixed = std::vector<Node*>();
			std::vector<Node*> k = kNearestNeighbours(Nodes,Nodes.at(0),6);
			NodeList.push_back(Node(k));

			for (int i = 0; i < Nodes.size(); i++) {
				if(!contain(k,Nodes.at(i)))
				{
					fixed.push_back(Nodes.at(i));
				}
			}


			newSet.push_back(&NodeList.back());
			Nodes = fixed;
		}
		BVH::build(newSet);
	}
}

std::vector<Node*> BVH::kNearestNeighbours(std::vector<Node*> Nodes, Node * refNode,int k) {
	std::vector<Node*> ref = Nodes;
	std::vector<Node*> triee;
	triee.reserve(k);

	//naive
	for(int i = ref.size()-1; i>=0;i--)
	{
		for(auto j = 0; j < i-1; j++)
		{
			float testj = Vector3::calcNorm(ref.at(j)->centroid - refNode->centroid);
			float testj1 = Vector3::calcNorm((ref.at(j+1))->centroid - refNode->centroid);
			if(testj1 < testj)
			{
				Node* n = ref.at(j);
				ref.at(j) = ref.at(j+1);
				ref.at(j+1) = n;
			}
		}
	}

	for (int i = 0; i < k; i++)
	{
		if(i < ref.size())
		{
			triee.push_back(ref.at(i));
		}
	}
return triee;
}
