/*
 * Box.cpp
 *
 *  Created on: Oct 5, 2019
 *      Author: huan
 */

#include "Box.hpp"


Box::Box(Vector3 min, Vector3 max) {
	this->min = min;
	this->max = max;

	center = (max+min) / 2;
	bounds[0] = min;
	bounds[1] = max;
}

Box::Box() {
	min = Vector3();
	max = Vector3();
	center = Vector3();
	bounds[0] = min;
	bounds[1] = max;
}

bool Box::intersect(Ray &r)
 {
	float tmin, tmax, tymin, tymax, tzmin, tzmax;

	if(r.dir == Vector3()) return false;

       	tmin = (bounds[r.sign[0]].x - r.pos.x) * r.invDir.x;
       	tmax = (bounds[1-r.sign[0]].x - r.pos.x) * r.invDir.x;

       	tymin = (bounds[r.sign[1]].y - r.pos.y) * r.invDir.y;
       	tymax = (bounds[1-r.sign[1]].y - r.pos.y) * r.invDir.y;

        if ((tmin > tymax) || (tymin > tmax))
            return false;
        if (tymin > tmin)
            tmin = tymin;
        if (tymax < tmax)
            tmax = tymax;


        tzmin = (bounds[r.sign[2]].z - r.pos.z) * r.invDir.z;
        tzmax = (bounds[1-r.sign[2]].z - r.pos.z) * r.invDir.z;

        if ((tmin > tzmax) || (tzmin > tmax))
                return false;
            if (tzmin > tmin)
                tmin = tzmin;
            if (tzmax < tmax)
                tmax = tzmax;

       return true;
}
/*
bool Box::intersect(Ray &r)
 {

	 Vector3 tmin = (min - r.pos) * r.invDir;
	 Vector3 tmax = (max - r.pos) * r.invDir;

	 float t0 = Vector3::min(tmin,tmax).maxComponent();
	 float t1 = Vector3::max(tmin,tmax).minComponent();

	 float dist;

	 if(t0 > 0) dist= t0;
	 else dist= t1;

	 return (t0<=t1) && (dist > 0);

}*/

HitInfo Box::intersectDebug(Ray &r)
 {
		float t = 0;
		HitInfo hit = HitInfo();
       	float tmin = (min.x - r.pos.x) * r.invDir.x;
       	float tmax = ( max.x - r.pos.x) * r.invDir.x;

       	if (tmin > tmax) std::swap(tmin, tmax);

       	float tymin = ( min.y - r.pos.y) * r.invDir.y;
       	float tymax = ( max.y - r.pos.y) *  r.invDir.y;

       	if (tymin > tymax) std::swap(tymin, tymax);

       	if ((tmin > tymax) || (tymin > tmax))
       	{
       		hit.hitSomething = false;
       		return hit;
       	}


       	if (tymin > tmin)
       		tmin = tymin;

       	if (tymax < tmax)
       		tmax = tymax;

       	float tzmin = ( min.z - r.pos.z) * r.invDir.z;
       	float tzmax = ( max.z - r.pos.z) * r.invDir.z;

       	if (tzmin > tzmax) std::swap(tzmin, tzmax);

       	if ((tmin > tzmax) || (tzmin > tmax))
		{
					hit.hitSomething = false;
					return hit;
		}

       	if (tzmin > tmin)
       		tmin = tzmin;

       	if (tzmax < tmax)
       		tmax = tzmax;

       t = tmin;

       if (t < 0) {
           t = tmax;
           if (t < 0)
		   {
               hit.hitSomething = false;
        	   return hit;
		   }
       }

       hit.hitSomething = true;
       hit.intersection = t;
       Material mat = Material(Color(1,1,1)*t*0.1,true,1);
       hit.material =mat;
       hit.normal = Vector3(1,1,1);

       return hit;
}


Box Box::boundingBox(std::vector<Box>* B) {

if(B->size()>=1)
{
//max
	Vector3 vMax = B->at(0).max;
	Vector3 vMin = B->at(0).min;

	std::vector<float> X = std::vector<float>();
	std::vector<float> Y = std::vector<float>();
	std::vector<float> Z = std::vector<float>();

	for(int i=0;i<B->size();i++)
	{
		Y.push_back(B->at(i).max.y);Y.push_back(B->at(i).min.y);
		X.push_back(B->at(i).max.x);X.push_back(B->at(i).min.x);
		Z.push_back(B->at(i).max.z);Z.push_back(B->at(i).min.z);
	}

	for(int i = 0; i< B->size()*2;i++)
	{
		if(vMin.x > X.at(i)) vMin.x = X.at(i);
		if(vMin.y > Y.at(i)) vMin.y = Y.at(i);
		if(vMin.z > Z.at(i)) vMin.z = Z.at(i);

		if(vMax.x < X.at(i)) vMax.x = X.at(i);
		if(vMax.y < Y.at(i)) vMax.y = Y.at(i);
		if(vMax.z < Z.at(i)) vMax.z = Z.at(i);
	}

	return Box(vMin,vMax);
}
else
{
	return Box();
}

	

}