/*
 * Triangle.cpp
 *
 *  Created on: Sep 1, 2019
 *      Author: huan
 */

#include "Triangle.hpp"

Triangle::Triangle() : a(Vector3()), b(Vector3()), c(Vector3()),normal(Vector3()), material(Material()){}

Triangle::Triangle(Vector3 a, Vector3 b, Vector3 c, Material material) :  a(a), b(b), c(c), material(material){
	this->normal = calcNormal();
}


Triangle::Triangle(Vector3 a, Vector3 b, Vector3 c, Vector3 normal, Material material) : a(a), b(b), c(c), normal(normal), material(material){
}

//___________________
//intersectTri

//utility fonction, compute the intersection point of a ray and with a tri
//output is the coord of the intersection point in ray space
HitInfo Triangle::intersect(Ray &r) {

	//Timer t {}; // statistics

	Vector3 u,v,k,n,d;
	u = b - a;
	v = c - a;
	k = r.dir;
	n=Vector3::cross(u,v);


	d = r.pos - a;

	Vector3 result(0,0,0);//resulting intersection, in base (u,v,k)
	float denominator = Vector3::dot(n,k);
	if(denominator != 0)
	{
		float invDen = 1/denominator;
		result.x = Vector3::dot(Vector3::cross(d,v),k)*invDen;
		result.y = Vector3::dot(Vector3::cross(u,d),k)*invDen;
		result.z = -Vector3::dot(n,d)*invDen;

		if(0<=result.x && result.x<=1 && 0<=result.y && result.y<=1 && result.z >= 0 && (result.x + result.y) <= 1)
		{
			//statCounter->addCriteriaTime(0,t.elapsed());
			return HitInfo(r,result.z,material,normal);
		}
		//not in the triangle
		else
		{
			//statCounter->addCriteriaTime(0,t.elapsed());
			return HitInfo();
		}
	}
	//don't intersect with plane
	else
	{
		//statCounter->addCriteriaTime(0,t.elapsed());
		return HitInfo();
	}
}

//___________________
//intersectTriMoller

HitInfo Triangle::intersectMoller(Ray &r) {

	float selfIntersectionThreshold = 0.0;

	Vector3 u,v;
	u = b - a;
	v = c - a;

	Vector3 pvec = Vector3::cross(r.dir,v);
	float det = Vector3::dot(u,pvec);

	// if the determinant is negative the triangle is backfacing
	// if the determinant is close to 0, the ray misses the triangle
	if (det < selfIntersectionThreshold) return HitInfo();
	// ray and triangle are parallel if det is close to 0
	//if (fabs(det) < selfIntersectionThreshold) return HitInfo();

	float invDet = 1.0 /det;
	Vector3 tvec = r.pos - a;
	float f = Vector3::dot(tvec,pvec) * invDet;

	if (f < 0 || f > 1) return HitInfo();

	Vector3 qvec = Vector3::cross(tvec,u);
	float g = Vector3::dot(r.dir,qvec) * invDet;
	if (g < 0 || f + g > 1) return HitInfo();

	float h = Vector3::dot(v,qvec) * invDet;

	return HitInfo(r,h,material,normal);
}

Vector3 Triangle::calcNormal() {
	return Vector3::cross(a,b);
}

Vector3 Triangle::calcCenter() {
	return (a +b +c)/3.0;
}
