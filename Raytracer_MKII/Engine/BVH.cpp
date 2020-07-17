/*
 * BVH.cpp
 *
 *  Created on: Feb 5, 2020
 *      Author: huan
 */

#include <BVH.hpp>


BVH::BVH() {
	NodeList = std::list<Node>();
	tree = 0;	
}

BVH::BVH(std::vector<Triangle> * triangleList) : triangleList(triangleList) {
	NodeList = std::list<Node>();
	NodeList.push_back(Node());
	for (int i = 0; i < triangleList->size(); i++)
	{
		NodeList.front().infoArray.push_back(triangleList->at(i));
	}
	
	NodeList.front().computeBoundingBox();
	build(&NodeList.front());
}


void BVH::build(Node * node) {
	if(node->infoArray.size()>2)//interior node
	{
		NodeList.push_back(Node());
		NodeList.push_back(Node());
		auto iter = NodeList.rbegin();
		Node *ref1 = &*iter;
		++iter;
		Node *ref2 = &*iter;
		
		node->split(ref2,ref1);//split into two child nodes previously created

		build(node->childR);
		build(node->childL);
	}
	else//leaf, end
	{
		node->computeBoundingBox();
		return;
	}
}

std::vector<Triangle> BVH::testRay(Ray &r) {
	std::vector<Triangle> tri = std::vector<Triangle>();
	testRay(r,&NodeList.front(),&tri);
	return tri;
}

int BVH::triangleAmount(Node *n)
{
	if(n == NULL)
	{
		return 0;
	}
	if(n->childL == NULL && n->childR==NULL)
	{
		return n->infoArray.size();
	}
	else
	{
		return triangleAmount(n->childL) + triangleAmount(n->childR);
	}
	
}

/*
std::vector<Box> BVH::testRayDEBUG(Ray r,int depthLim) {
	std::vector<Box> result = std::vector<Box>();
	result.reserve(triangleList->size());
	return testRayDEBUG(r,tree,result,0,depthLim);
}*/


void BVH::testRay(Ray &r, Node *n, std::vector<Triangle> * result) {
	if(n->box.intersect(r))
	{
		if(!n->infoArray.empty() == 0)//interior node
		{
			testRay(r,n->childR,result);
			testRay(r,n->childL,result);
		}
		else
		{
			for (int i = 0; i < n->infoArray.size(); i++)
			{
				result->push_back(n->infoArray.at(i));
			}
		}
	}
	return;
}

/*
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
}*/
