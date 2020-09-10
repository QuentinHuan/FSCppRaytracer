#ifndef MESH_H
#define MESH_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include "ObjImporter.h"

class mesh
{
private:
    /* data */
public:
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;
    ObjImporter objData;

    mesh(std::string filename)
    {
        //VAO
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        //data extraction
        objData = ObjImporter(filename);

        //VBO
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, objData.vertexSize, objData.vertex, GL_STATIC_DRAW);
        //EBO
        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, objData.indicesSize, objData.indices, GL_STATIC_DRAW);

        //attribute pointer: x,y,z
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);
        //attribute pointer: x,y,z
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(1);
    }
};

#endif