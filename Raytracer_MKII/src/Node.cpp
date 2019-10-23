/*
 * Node.cpp
 *
 *  Created on: Oct 3, 2019
 *      Author: huan
 */

#include "Node.hpp"



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

//range les triangles de la node parente dans la node fille
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

//test si le triangle est du coté direct ou indirect
//renvoie 1 pour completement dedans, 0 pour chevauchement, -1 pour dehors
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

