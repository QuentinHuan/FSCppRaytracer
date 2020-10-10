/*
 * Ray.cpp
 *
 *  Created on: Sep 1, 2019
 *      Author: huan
 */

#include "Ray.hpp"

Ray::Ray() : dir(Vector3()), pos(Vector3()), invPDF(0) {}

Ray::Ray(Vector3 dir, Vector3 pos) : dir(dir), pos(pos)
{
    invDir = Vector3(1.0 / dir.x, 1.0 / dir.y, 1.0 / dir.z);
    sign[0] = (invDir.x < 0);
    sign[1] = (invDir.y < 0);
    sign[2] = (invDir.z < 0);
    invPDF = 0;
}

Ray::Ray(Vector3 dir, Vector3 pos, float invPDF) : dir(dir), pos(pos), invPDF(invPDF)
{
    invDir = Vector3(1.0 / dir.x, 1.0 / dir.y, 1.0 / dir.z);
    sign[0] = (invDir.x < 0);
    sign[1] = (invDir.y < 0);
    sign[2] = (invDir.z < 0);
}
