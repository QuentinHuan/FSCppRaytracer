/*
 * Node.cpp
 *
 *  Created on: Oct 3, 2019
 *      Author: huan
 */

#include "Node.hpp"
//___________________
//Constructors:

Node::Node() {
	triangles = std::vector<Triangle>();
	leaf = false;
	origin = false;
	father = this;
	rChild = 0;
	lChild = 0;
	scale = 1;
	direct = true;
	depth = getDepth();
}

Node::Node(Node *father, bool leaf, bool origin, bool direct, float scale) {
	//triangles = list;
	this->leaf = leaf;
	this->origin = origin;
	this->father = father;
	rChild = 0;
	lChild = 0;
	depth = getDepth();
	this->scale = scale;
	this->direct = direct;
	treeCenter = father->treeCenter;
	addPlane(direct);
	addBox(direct);
	sort();
	numberFaces = triangles.size();

}

Node::Node(std::vector<Triangle> list, Node *father, bool leaf, bool origin, bool direct, float scale, Vector3 center) {
	triangles = list;
	this->leaf = leaf;
	this->origin = origin;
	this->father = father;
	rChild = 0;
	lChild = 0;
	this->scale = scale;
	this->direct = direct;
	addPlane(direct);
	addBox(direct);
	depth = 1;
	treeCenter = center;
}

//___________________
//TreeBuilding:
/*sort the triangles of the father node into the child node
 *tris on the direct side of the plane goes into the right child
 *tris on the indirect into the left child
 *tris that are overlapping stay in the father node
 */
void Node::sort() {
	std::vector<Triangle> T = std::vector<Triangle>();
	for(int i=0;i<father->triangles.size();i++)
	{
		Triangle t = father->triangles.at(i);
		int test = isTriangleInside(t);
		if(test == 1)
		{
			triangles.push_back(t);
		}
		if(test == 0)
		{
			T.push_back(t);
		}
		if(test == -1)
		{
			T.push_back(t);
		}
	}
	father->triangles = T;
	return;
}


//test which side of the plane a triangle is
//return 1 for direct side, 0 for overlapping, -1 for indirect side
int Node::isTriangleInside(Triangle t) {
	bool bA,bB,bC;

	bA = isVector3Inside(t.a);
	bB = isVector3Inside(t.b);
	bC = isVector3Inside(t.c);

	if(bA && bB && bC)
	{
		return 1;
	}
	else
	{
		if(bA || bB || bC)
		{
			return 0;
		}
		else
		{
			return -1;
		}
	}
}

//test which side of the plane a point is
//return true for direct side, 0 for indirect side
bool Node::isVector3Inside(Vector3 t) {

	if(direct)
	{
		if(Vector3::dot(father->planeNormal, t-father->planeOrigin) >= 0) return true;
			else return false;
	}
	else
	{
		if(Vector3::dot(father->planeNormal, t-father->planeOrigin) <= 0) return true;
			else return false;
	}

}

//build the box associated with the node from the parent node's box and the dividing plane
void Node::addBox(bool direct) {
	if(!origin)
	{
		int coef = (depth+1)/3;
		if(direct)
		{
			Vector3 min = father->box.min + father->planeNormal*std::pow(0.5,coef)*scale;
			Box b = Box(min, father->box.max);
			box = b;
		}
		else
		{
			Vector3 max = father->box.max - father->planeNormal*std::pow(0.5,coef)*scale;
			Box b = Box( father->box.min, max);
			box = b;
		}
	}
	else
	{
		Vector3 min = Vector3(1,1,1)*(-1)*scale;
		Vector3 max = Vector3(1,1,1)*scale;
		Box b = Box(min, max);
		box = b;
	}
}

//add the dividing plane, computed from the parent node
void Node::addPlane(bool direct) {
	if(!origin)
	{
		float sign = 1;
		if(!direct) sign = -1;

		planeOrigin = father->planeOrigin + father->planeNormal*sign*std::pow(0.5,depth)*scale;
		switch((depth)%3)
		{
		case 0:
			planeNormal = Vector3(0,1,0);
			break;
		case 1:
			planeNormal = Vector3(0,0,1);
			break;
		case 2:
			planeNormal = Vector3(1,0,0);
			break;
		}
	}
	else
	{
		planeOrigin = treeCenter;
		planeNormal = Vector3(0,0,1);
	}
}

//compute the depth of the node
int Node::getDepth() {
	if(!origin)
	{
		return 1 + father->depth;
	}
	else
	{
		return 1;
	}
}
