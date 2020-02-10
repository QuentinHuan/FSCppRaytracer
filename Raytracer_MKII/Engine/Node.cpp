/*
 * Node.cpp
 *
 *  Created on: Feb 5, 2020
 *      Author: huan
 */

#include <Node.hpp>


Node::Node(Node *parent, Triangle triangle) : parent(parent), triangle(triangle){
	centroid = triangle.calcCenter();

	std::vector<Triangle> T = std::vector<Triangle>();
	T.push_back(triangle);
	box = Triangle::boundingBox(T);
}

Node::Node(std::vector<Node*> childs) : childs(childs), triangle(),parent(0) {

	if(childs.size() != 0)
	{
		std::vector<Box> B = std::vector<Box>();
		B.reserve(childs.size());
		for (int i = 0; i < childs.size(); i++) {
			centroid = centroid + childs.at(i)->centroid;
			B.push_back(childs.at(i)->box);
		}
		centroid = centroid / childs.size();

		box = Box::boundingBox(B);
	}
	else
	{
		centroid = Vector3();
		box = Box();
	}
}

std::string Node::toString() {
	toString(this, "");
	return "";
}

void Node::toString(Node *n, std::string s) {

	for(int i =0; i<n->childs.size();i++)
	{
		toString(n->childs.at(i),s);
	}
	if(n->childs.size() == 0)
	{
		//s = s + std::to_string(n->triangle.a.x) + "\n";
		//return s;
		std::printf("%s\n",std::to_string(n->triangle.a.x).c_str());
	}

}
