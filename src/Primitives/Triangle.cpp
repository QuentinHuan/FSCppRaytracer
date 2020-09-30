/*
 * Triangle.cpp
 *
 *  Created on: Sep 1, 2019
 *      Author: huan
 */

#include "Triangle.hpp"

Triangle::Triangle() : a(Vector3()), b(Vector3()), c(Vector3()), normal(Vector3()), material(Material())
{
	bBox = Box(Vector3::min(a, Vector3::min(b, c)), Vector3::max(a, Vector3::max(b, c)));
}

Triangle::Triangle(Vector3 a, Vector3 b, Vector3 c, Material material) : a(a), b(b), c(c), material(material)
{
	this->normal = calcNormal();
	bBox = Box(Vector3::min(a, Vector3::min(b, c)), Vector3::max(a, Vector3::max(b, c)));
}

Triangle::Triangle(Vector3 a, Vector3 b, Vector3 c, Vector3 normal, Material material) : a(a), b(b), c(c), normal(normal), material(material)
{
	bBox = Box(Vector3::min(a, Vector3::min(b, c)), Vector3::max(a, Vector3::max(b, c)));
}

//___________________
//intersectTri

//utility function, compute the intersection point of a ray and with a tri
//output is the coord of the intersection point in ray space
HitInfo Triangle::intersect(Ray &r)
{
	const float EPSILON = 0.0000001;
	Vector3 vertex0 = a;
	Vector3 vertex1 = b;
	Vector3 vertex2 = c;
	Vector3 edge1, edge2, h, s, q;
	float a, f, u, v;
	edge1 = vertex1 - vertex0;
	edge2 = vertex2 - vertex0;
	h = Vector3::cross(r.dir, edge2);
	a = Vector3::dot(edge1, h);
	if (a > -EPSILON && a < EPSILON)
		return HitInfo(); // This ray is parallel to this triangle.
	f = 1.0 / a;
	s = r.pos - vertex0;
	u = f * Vector3::dot(s, h);
	if (u < 0.0 || u > 1.0)
		return HitInfo();
	q = Vector3::cross(s, edge1);
	v = f * Vector3::dot(r.dir, q);
	if (v < 0.0 || u + v > 1.0)
		return HitInfo();
	// At this stage we can compute t to find out where the intersection point is on the line.
	float t = f * Vector3::dot(edge2, q);
	if (t > EPSILON) // ray intersection
	{
		return HitInfo(r, t, material, normal);
	}
	else // This means that there is a line intersection but not a ray intersection.
		return HitInfo();
}

/* 
	//Timer t {}; // statistics

	if (r.dir == Vector3())
		return HitInfo();

	Vector3 u, v, k, n, d;
	u = b - a;
	v = c - a;
	k = r.dir.normalize();
	n = Vector3::cross(u, v);

	n = calcNormal();

	d = r.pos - a;

	Vector3 result(0, 0, 0); //resulting intersection, in base (u,v,k)
	float denominator = Vector3::dot(n, k);
	if (denominator != 0)
	{
		float invDen = 1.0f / denominator;
		result.x = Vector3::dot(Vector3::cross(d, v), k) * invDen;
		result.y = Vector3::dot(Vector3::cross(u, d), k) * invDen;
		result.z = -Vector3::dot(n, d) * invDen;

		if (0 <= result.x && result.x <= 1 && 0 <= result.y && result.y <= 1 && result.z >= 0 && (result.x + result.y) <= 1)
		{
			//statCounter->addCriteriaTime(0,t.elapsed());
			return HitInfo(r, result.z, material, normal);
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
	} */

//___________________
//intersectTriMoller

HitInfo Triangle::intersectMoller(Ray &r)
{

	float selfIntersectionThreshold = 0.000000001;

	Vector3 u, v;
	u = b - a;
	v = c - a;

	Vector3 pvec = Vector3::cross(r.dir, v);
	float det = Vector3::dot(u, pvec);

	// if the determinant is negative the triangle is backfacing
	// if the determinant is close to 0, the ray misses the triangle
	//if (det < selfIntersectionThreshold) return HitInfo();
	// ray and triangle are parallel if det is close to 0
	if (fabs(det) < selfIntersectionThreshold)
		return HitInfo();

	float invDet = 1.0 / det;
	Vector3 tvec = r.pos - a;
	float f = Vector3::dot(tvec, pvec) * invDet;

	if (f < 0 || f > 1)
		return HitInfo();

	Vector3 qvec = Vector3::cross(tvec, u);
	float g = Vector3::dot(r.dir, qvec) * invDet;
	if (g < 0 || f + g > 1)
		return HitInfo();

	float h = Vector3::dot(v, qvec) * invDet;

	return HitInfo(r, h, material, normal);
}

Vector3 Triangle::calcNormal()
{
	return Vector3::cross(b - a, c - a);
}

Vector3 Triangle::calcCenter()
{
	return (a + b + c) / 3.0;
}

Box Triangle::boundingBox(std::vector<Triangle> T)
{
	//max
	Vector3 vMax = T.at(0).a;
	Vector3 vMin = T.at(0).a;

	std::vector<float> X;
	std::vector<float> Y;
	std::vector<float> Z;

	for (auto it = T.begin(); it != T.end(); it++)
	{
		X.push_back(it->a.x);
		X.push_back(it->b.x);
		X.push_back(it->c.x);
		Y.push_back(it->a.y);
		Y.push_back(it->b.y);
		Y.push_back(it->c.y);
		Z.push_back(it->a.z);
		Z.push_back(it->b.z);
		Z.push_back(it->c.z);
	}

	for (int i = 0; i < T.size() * 3; i++)
	{
		if (vMin.x > X.at(i))
			vMin.x = X.at(i);
		if (vMin.y > Y.at(i))
			vMin.y = Y.at(i);
		if (vMin.z > Z.at(i))
			vMin.z = Z.at(i);

		if (vMax.x < X.at(i))
			vMax.x = X.at(i);
		if (vMax.y < Y.at(i))
			vMax.y = Y.at(i);
		if (vMax.z < Z.at(i))
			vMax.z = Z.at(i);
	}

	return Box(vMin, vMax);
}

float Triangle::area()
{

	Vector3 u = a - b, v = a - c;
	return Vector3::calcNorm(Vector3::cross(u, v)) / 2;
}
