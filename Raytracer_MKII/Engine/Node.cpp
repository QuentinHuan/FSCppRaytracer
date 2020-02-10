/*
 * Node.cpp
 *
 *  Created on: Feb 5, 2020
 *      Author: huan
 */

#include <Node.hpp>


Node::Node(Node *parent, Triangle triangle) : parent(parent), triangle(triangle){
	centroid = triangle.calcCenter();
}

Node::Node(std::vector<Node*> childs) : childs(childs), triangle(),parent(0) {

	if(childs.size() != 0)
	{
		for (int i = 0; i < childs.size(); i++) {
			centroid = centroid + childs.at(i)->centroid;
		}
		centroid = centroid / childs.size();
	}
	else centroid = Vector3();
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
