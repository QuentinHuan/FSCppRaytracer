/*
 * Node.cpp
 *
 *  Created on: Feb 5, 2020
 *      Author: huan
 */

#include <Node.hpp>


Node::Node(Node *parent, Triangle triangle) {
	this->triangle = triangle;
	this->parent = parent;

}

Node::Node(std::vector<Node*> childs) {
	this->childs = childs;
	triangle = Triangle();
	this->parent = 0;
}
