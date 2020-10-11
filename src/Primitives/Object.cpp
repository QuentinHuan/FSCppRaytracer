/*
 * Object.cpp
 *
 *  Created on: Sep 3, 2019
 *      Author: huan
 */

#include "Object.hpp"

Object::Object()
{
}

Object::Object(std::vector<Triangle> triArray)
{
	faces = triArray;
}

Object::Object(std::string file)
{
	mat.push_back(Material(Color(1.0f, 1.0f, 1.0f), true, 0));
	faces = load(file).faces;
}

Object Object::load(std::string filename)
{
	std::ifstream file{filename};
	std::string line = "";

	std::vector<Vector3> fileVertexArray; //vertex collection described in the file
	std::vector<Vector3> fileNormalArray; //normal vector collection described in the file
	std::vector<Triangle> objectTriArray; //collection of the triangles composing the object

	int matCounter = 0;
	if (file.is_open())
	{
		while (std::getline(file, line))
		{
			//create vertex collection

			//file line example:
			//" v 1.000000 1.000000 -1.000000 "
			// x|oooooooo|oooooooo|ooooooooo
			// 'o' are what to keep, | are splits
			// x are discarded
			if (line.at(0) == 'v')
			{
				//geometric vertex
				if (line.at(1) == ' ')
				{
					std::vector<std::string> elements;
					Utility::split(line, elements, ' ');
					std::vector<float> coord;
					for (int i = 1; i < 4; i++)
					{
						coord.push_back(std::stof(elements.at(i)));
					}
					fileVertexArray.push_back(Vector3{coord.at(0), coord.at(1), coord.at(2)});
				}
				//vertex texture
				if (line.at(1) == 't')
				{
				}
				//vertex normal
				if (line.at(1) == 'n')
				{
					std::vector<std::string> elements;
					Utility::split(line, elements, ' ');
					std::vector<float> coord;
					for (int i = 1; i < 4; i++)
					{
						coord.push_back(std::stof(elements.at(i)));
					}
					fileNormalArray.push_back(Vector3{coord.at(0), coord.at(1), coord.at(2)});
				}
			}

			if (line.at(0) == 'u')
			{
				switch (matCounter)
				{
				case 0:
					mat.push_back(Material(Color(1, 1, 1), 0.05));
					break;
				case 1:
					mat.push_back(Material(Color(1, 0, 0), 1));
					break;
				case 2:
					mat.push_back(Material(Color(0, 1, 0), 1));
					break;
				case 3:
					mat.push_back(Material(Color(1.0f, 1.0f, 1.0f), true, 10));
					break;
				case 4:
					mat.push_back(Material(Color(1, 1, 1), 1));
					break;
				}
				matCounter++;
			}

			//create triangles using the vertex inside the collection
			if (line.at(0) == 'f')
			{
				//file line example:
				//" f 5/1/1 3/2/1 1/3/1"
				//  x|VxTxN|VxTxN|VxTxN

				std::vector<int> index; //index of the 3 vertices to use
				int normalIndex;

				std::vector<std::string> splitOnSpace;
				Utility::split(line, splitOnSpace, ' ');

				for (int i = 1; i < 4; i++)
				{
					std::vector<std::string> splitOnSlash;
					Utility::split(splitOnSpace.at(i), splitOnSlash, '/');
					index.push_back(std::stof(splitOnSlash.at(0)) - 1); //-1 as .obj vertex collection starts with vertex number 1
					normalIndex = std::stoi(splitOnSlash.at(2)) - 1;
				}

				//tri made of the indices
				Triangle tri = Triangle(fileVertexArray.at(index.at(0)), fileVertexArray.at(index.at(1)), fileVertexArray.at(index.at(2)), fileNormalArray.at(normalIndex), matCounter);
				objectTriArray.push_back(tri);
			}
		}
	}
	return Object{objectTriArray};
}
