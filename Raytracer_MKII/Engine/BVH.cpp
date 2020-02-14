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

int BVH::numberOfNode(Node * n, int numb)
{

	for(auto it = n->childs.begin(); it != n->childs.end();it++)
	{
		numb = numberOfNode(*it,numb);
	}

		numb = numb + 1;

	return numb;
}

bool contain(std::vector<Node*> &A, Node* &B)
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
		while(Nodes.size() != 0)
		{
			std::vector<Node*> fixed = std::vector<Node*>();
			std::vector<Node*> k = kNearestNeighbours(Nodes,Nodes.at(0),2);
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
	if(Nodes.size() > k+1)
	{
		for(int i = ref.size()-1; i>=ref.size()-1-k;i--)
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

std::vector<Triangle*> BVH::testRay(Ray &r) {
	std::vector<Triangle*> result = std::vector<Triangle*>();
	result.reserve(triangleList.size());
	testRay(r,tree,&result);
	return result;
}


std::vector<Box> BVH::testRayDEBUG(Ray r,int depthLim) {
	std::vector<Box> result = std::vector<Box>();
	result.reserve(triangleList.size());
	return testRayDEBUG(r,tree,result,0,depthLim);
}

std::vector<Triangle*> BVH::testRay(Ray &r, Node *n, std::vector<Triangle*> * result) {

	if(n->box.intersect(r))
	{
		if(n->childs.size() != 0)
		{
			for(auto it = n->childs.begin();it != n->childs.end();it++)
			{
				testRay(r,*it,result);
			}
		}
		else
		{
			result->push_back(&n->triangle);
		}
	}
	return *result;
}

std::vector<Box> BVH::BVH::testRayDEBUG(Ray r, Node *n,std::vector<Box> result,int depth,int depthLim) {


	float t;
	if(n->box.intersect(r))
	{
		if(depth < depthLim)
		{
			for(auto it = n->childs.begin();it != n->childs.end();it++)
			{
				result = testRayDEBUG(r,*it,result,depth+1,depthLim);
			}
		}
		else
		{
			result.push_back(n->box);
		}
	}
	return result;

	/*
	//Box b = Box(Vector3(0,0,0),Vector3(-0.2,1,-0.5));
	std::vector<Triangle> T = std::vector<Triangle>();
	T.push_back(triangleList.at(2));

	std::vector<Box> B = std::vector<Box>();
	B.push_back(tree->childs.at(1)->box);
	B.push_back(tree->childs.at(0)->box);
	Box b = Box::boundingBox(B);
	result.push_back(tree->childs.at(1)->box);
	return result;*/

}
