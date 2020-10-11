/*
 * Node.cpp
 *
 *  Created on: Feb 5, 2020
 *      Author: huan
 */

#include <Node.hpp>

Node::Node()
{
    infoArray = std::vector<Triangle>();
    childL = nullptr;
    childR = nullptr;
    primOffset = 0;
    nPrimitives = 0;
    centroid = Vector3();
    box = Box();
}

Node::Node(std::vector<Triangle> infoArray) : infoArray(infoArray)
{
    childL = nullptr;
    childR = nullptr;
    primOffset = 0;
    nPrimitives = 0;
    centroid = Vector3();
    box = Box();
}

Node::Node(int primOffset, int nPrimitives) : primOffset(primOffset), nPrimitives(nPrimitives)
{
    infoArray = std::vector<Triangle>();
    childL = nullptr;
    childR = nullptr;
    centroid = Vector3();
    box = Box();
}

void Node::split(Node *child1, Node *child2)
{
    if (infoArray.size() > 2)
    {
        //--------split method---------------//
        //compute primitives centroids variance along each axis
        Vector3 variance = Vector3();
        Vector3 mean = Vector3();
        for (int i = 0; i < infoArray.size(); i++)
        {
            mean = mean + infoArray.at(i).bBox.center;
        }
        mean = mean * (1.0 / infoArray.size()); //mean

        for (int i = 0; i < infoArray.size(); i++)
        {
            variance = variance + ((infoArray.at(i).bBox.center - mean) * (infoArray.at(i).bBox.center - mean));
        }
        variance = variance * (1.0 / infoArray.size());

        //split onlong the maximum variance axis, on the mean centroid value
        //find the axis
        int split = 1; //1:x, 2:y, 3:z
        if (variance.x > variance.y)
        {
            if (variance.x > variance.z)
            {
                split = 1;
            }
            else
            {
                split = 3;
            }
        }
        else
        {
            if (variance.y > variance.z)
            {
                split = 2;
            }
            else
            {
                split = 3;
            }
        }
        splitOn = split;
        float pivot = mean.getComponent(split);
        //--------split method---------------//

        //split along the axis
        std::vector<Triangle> infoChildR = std::vector<Triangle>();
        std::vector<Triangle> infoChildL = std::vector<Triangle>();
        for (int i = 0; i < infoArray.size(); i++)
        {
            if (infoArray.at(i).bBox.center.getComponent(split) < pivot)
            {
                infoChildL.push_back(infoArray.at(i));
            }
            else
            {
                infoChildR.push_back(infoArray.at(i));
            }
        }

        //assign each children its primitives
        child1->infoArray.assign(infoChildR.begin(), infoChildR.end());
        child2->infoArray.assign(infoChildL.begin(), infoChildL.end());

        //compute new bounding boxes
        child1->computeBoundingBox();
        child2->computeBoundingBox();

        //assign node its children
        childR = child1;
        childL = child2;

        //erase the now empty interrior node
        infoArray.clear();
        infoArray.shrink_to_fit();
        return;
    }
    else
    {
        if (infoArray.size() == 2)
        {
            computeBoundingBox();
            return;
        }
        if (infoArray.size() == 1)
        {
            box = infoArray.at(0).bBox;
            return;
        }
        else
        {
            return;
        }
    }
}

void Node::computeBoundingBox()
{
    std::vector<Box> BB = std::vector<Box>();
    for (int i = 0; i < infoArray.size(); i++)
    {
        BB.push_back(infoArray.at(i).bBox);
    }
    if (!BB.empty())
    {
        box = Box::boundingBox(&BB);
    }
    else
    {
        box = Box();
    }
}