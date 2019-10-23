/*
 * BSP.cpp
 *
 *  Created on: Oct 3, 2019
 *      Author: huan
 */
#include "BSP.hpp"


BSP::BSP(int maxDepth, std::vector<Triangle> allTriangle, float scale, Vector3 offset) {
	this->maxDepth = maxDepth;
	this->allTriangle = allTriangle;
	this->scale = scale;
	nodeList = std::vector<Node>();
	Node origin = Node(allTriangle,0,false,true,true,scale,offset);
	nodeList.push_back(origin);
	buildBSPTree(&origin);

	BSPTree = origin;
	partition = std::vector<Triangle>();
	counter = 1;
}



Node* BSP::buildBSPTree(Node *n) {

	int depth = n->depth;
	if(depth >= maxDepth)
	{
		n->rChild = new Node(n,true,false,true,scale);
		n->lChild = new Node(n,true,false,false,scale);
		return n;
	}
	else
	{
		n->rChild = new Node(n,false,false,true,scale);
		n->lChild = new Node(n,false,false,false,scale);

		buildBSPTree(n->rChild);
		buildBSPTree(n->lChild);

		return n;
	}
	return n;
}

std::vector<Triangle> BSP::debugPartition() {

	partition = std::vector<Triangle>();
	getTriangles(&BSPTree);

	return partition;
}


void BSP::getTriangles(Node *n) {

	int numberLeaves = std::pow(2,maxDepth)+1;
	//float c=std::rand();

	if(n != NULL)
	{
		if(n->triangles.size() != 0)
		{
			float c = counter;

			Material mat;
			if((int)c%2 == 0)
			{
				mat = Material(Color(0,c/numberLeaves,c/numberLeaves),true,1);
			}
			else
			{
				mat = Material(Color(c/numberLeaves,0,c/numberLeaves),true,1);
			}

			for(Triangle t : n->triangles)
			{
				t.material = mat;
				partition.push_back(t);
			}
			counter++;
		}
		getTriangles(n->rChild);
		getTriangles(n->lChild);
	}
}

bool BSP::intersectBox(Ray r, Box b)
{
	float tmin = (b.min.x - r.pos.x) / r.dir.x;
	float tmax = (b.max.x - r.pos.x) / r.dir.x;

	if (tmin > tmax) std::swap(tmin, tmax);

	float tymin = (b.min.y - r.pos.y) / r.dir.y;
	float tymax = (b.max.y - r.pos.y) / r.dir.y;

	if (tymin > tymax) std::swap(tymin, tymax);

	if ((tmin > tymax) || (tymin > tmax))
		return false;

	if (tymin > tmin)
		tmin = tymin;

	if (tymax < tmax)
		tmax = tymax;

	float tzmin = (b.min.z - r.pos.z) / r.dir.z;
	float tzmax = (b.max.z - r.pos.z) / r.dir.z;

	if (tzmin > tzmax) std::swap(tzmin, tzmax);

	if ((tmin > tzmax) || (tzmin > tmax))
		return false;

	if (tzmin > tmin)
		tmin = tzmin;

	if (tzmax < tmax)
		tmax = tzmax;

	return true;
}

std::vector<Triangle> BSP::accelerationStructure(Ray r) {
	partition = std::vector<Triangle>();
	getTrianglesHitByRay(&BSPTree,r);
	return partition;
}

BSP::BSP() {
		this->maxDepth = 0;
		this->allTriangle = std::vector<Triangle>();
		this->scale = 1;
		nodeList = std::vector<Node>();
		partition = std::vector<Triangle>();
}

void BSP::getTrianglesHitByRay(Node *n, Ray r) {

	if(n != NULL)
	{
		if(intersectBox(r,n->box) && n->triangles.size() != 0) //leaf == true
		{
			for(Triangle t : n->triangles)
			{
				partition.push_back(t);
			}
			//return;
		}
		getTrianglesHitByRay(n->rChild,r);
		getTrianglesHitByRay(n->lChild,r);
	}
	else
	{
		return;
	}
}
