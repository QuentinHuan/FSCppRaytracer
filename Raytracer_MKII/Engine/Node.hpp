/*
 * Node.hpp
 *
 *  Created on: Feb 5, 2020
 *      Author: huan
 */
#pragma once
#ifndef NODE_HPP_
#define NODE_HPP_
#include <vector>
#include <string>
#include "Triangle.hpp"
#include "Box.hpp"

class Node {
public:

	Node* parent;
	std::vector<Node*> childs;

	Triangle triangle;
	Vector3 centroid;
	Box box;

	Node(Node* parent, Triangle triangle);
	Node(std::vector<Node*> childs);
	std::string toString();

private:
	std::string toString(Node * n, std::string s);


};

#endif /* NODE_HPP_ */
