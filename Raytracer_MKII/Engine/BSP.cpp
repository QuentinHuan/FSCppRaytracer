/*
 * BSP.cpp
 *
 *  Created on: Oct 3, 2019
 *      Author: huan
 */
#include "BSP.hpp"

//___________________
//Constructors
/*
 * maxDepth: maximum depth of the bsp tree
 * allTriangle: list of all the triangles of the scene
 * scale: the scale of the bounding box of the whole scene centered in (0,0,0)
 * offset: center bounding box of the whole scene centered in (0,0,0)
 */
BSP::BSP(int maxDepth, std::vector<Triangle> allTriangle, float scale, Vector3 offset) {
	this->maxDepth = maxDepth;
	this->allTriangle = allTriangle;
	this->scale = scale;

	Node origin = Node(allTriangle,0,false,true,true,scale,offset);
	buildBSPTree(&origin);

	BSPTree = origin;//public interface to read the BSP tree
	result = std::vector<Triangle>();
	counter = 1;
}


BSP::BSP() {
	this->maxDepth = 0;
	this->allTriangle = std::vector<Triangle>();
	this->scale = 1;
	nodeList = std::vector<Node>();
	result = std::vector<Triangle>();
}

//___________________
//Tree Building:
//recursively build the BSP tree, from the triangle list given to the root node
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

//___________________
//Compute acceleration result:
//for a given ray, return the triangles that may be hit
std::vector<Triangle> BSP::accelerationStructure(Ray r) {
	result = std::vector<Triangle>();

	getTrianglesHitByRay(&BSPTree,r);
	return result;
}

/*
 * 			if(distance <= minDistanceFromOrigin)
			{
				minDistanceFromOrigin = distance;
 */

void BSP::getTrianglesHitByRay(Node *n, Ray r) {

	if(n != NULL)
	{
		if(n->origin)
		{
			minDistanceFromOrigin = 1000;
		}
		float distance = 0;
		if(n->box.intersect(r,distance)) //leaf == true
		{
			if(distance <= minDistanceFromOrigin)
			{
				minDistanceFromOrigin = distance;
				if(n->triangles.size() != 0)
				{
					for(Triangle t : n->triangles)
					{
						result.push_back(t);
					}
				}
			}
		}
		getTrianglesHitByRay(n->rChild,r);
		getTrianglesHitByRay(n->lChild,r);
	}
	else
	{
		return;
	}
}

//___________________
//DEBUG
//give one color per space partition
std::vector<Triangle> BSP::debugPartition() {

	result = std::vector<Triangle>();
	getTriangles(&BSPTree);

	return result;
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
				result.push_back(t);
			}
			counter++;
		}
		getTriangles(n->rChild);
		getTriangles(n->lChild);
	}
}
