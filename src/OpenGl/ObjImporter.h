#ifndef OBJIMPORTER_H
#define OBJIMPORTER_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

class ObjImporter
{
private:
    /* data */

public:
    float *vertex;
    float *normal;
    int *indices;

    unsigned int vertexSize;
    unsigned int normalSize;
    unsigned int indicesSize;

    std::vector<float> fileVertexArray; //vertex collection described in the file
    std::vector<float> fileNormalArray; //normal vector collection described in the file
    std::vector<int> elementArray;      //collection of the triangles composing the object

    ObjImporter()
    {
    }

    ObjImporter(std::string filename)
    {
        std::ifstream file{filename};
        std::string line = "";

        int matCounter = 0;
        try
        {
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
                            split(line, elements, ' ');
                            std::vector<float> coord;
                            for (int i = 1; i < 4; i++)
                            {
                                fileVertexArray.push_back(std::stof(elements.at(i)));
                            }
                        }
                        //vertex texture
                        if (line.at(1) == 't')
                        {
                        }
                        //vertex normal
                        if (line.at(1) == 'n')
                        {
                            std::vector<std::string> elements;
                            split(line, elements, ' ');
                            std::vector<float> coord;
                            for (int i = 1; i < 4; i++)
                            {
                                fileNormalArray.push_back(std::stof(elements.at(i)));
                            }
                        }
                    }
                    //materials
                    if (line.at(0) == 'u')
                    {
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
                        split(line, splitOnSpace, ' ');

                        for (int i = 1; i < 4; i++)
                        {
                            std::vector<std::string> splitOnSlash;
                            split(splitOnSpace.at(i), splitOnSlash, '/');
                            index.push_back(std::stof(splitOnSlash.at(0)) - 1); //-1 as .obj vertex collection starts with vertex number 1
                            normalIndex = std::stoi(splitOnSlash.at(2)) - 1;
                        }

                        //tri made of the indices
                        for (int i = 0; i < 3; i++)
                        {
                            elementArray.push_back(index.at(i));
                        }
                    }
                }
            }
        }

        catch (std::ifstream::failure &e)
        {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        }

        vertex = &fileVertexArray.at(0);
        normal = &fileNormalArray.at(0);
        indices = &elementArray.at(0);

        vertexSize = sizeof(float) * fileVertexArray.size();
        normalSize = sizeof(float) * fileNormalArray.size();
        indicesSize = sizeof(int) * elementArray.size();
    }

    size_t
    split(const std::string &txt, std::vector<std::string> &strs, char ch)
    {
        size_t pos = txt.find(ch);
        size_t initialPos = 0;
        strs.clear();

        // Decompose statement
        while (pos != std::string::npos)
        {
            strs.push_back(txt.substr(initialPos, pos - initialPos));
            initialPos = pos + 1;

            pos = txt.find(ch, initialPos);
        }

        // Add the last one
        strs.push_back(txt.substr(initialPos, std::min(pos, txt.size()) - initialPos + 1));

        return strs.size();
    }
};

#endif