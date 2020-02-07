/*
 * Node.hpp
 *
 *  Created on: Feb 5, 2020
 *      Author: huan
 */

#ifndef NODE_HPP_
#define NODE_HPP_
#include <vector>
#include "Triangle.hpp"

class Node {
public:

	Node* parent;
	std::vector<Node*> childs;

	Triangle triangle;

	Node(Node* parent, Triangle triangle);
	Node(std::vector<Node*> childs);



};

#endif /* NODE_HPP_ */
