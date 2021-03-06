/*
 * Material.hpp
 *
 *  Created on: Sep 14, 2019
 *      Author: huan
 */
#pragma once
#ifndef MATERIAL_HPP_
#define MATERIAL_HPP_
#include "Color.hpp"
#include "Vector3.hpp"
#include "Quaternion.hpp"
#include "Ray.hpp"
#include <random>

class Material
{
public:
	Material();
	std::string type;
	virtual Color BRDF(Vector3 v, Vector3 l, Vector3 n) = 0;
	virtual Color getBaseColor() = 0;
	virtual Ray sampleBRDF(Vector3 n, Vector3 v, Vector3 position, std::default_random_engine &generator, std::uniform_real_distribution<float> &distribution) = 0;
};

class Mat_Lambert : public Material
{
public:
	Color diffuse;

	Mat_Lambert(Color diffuse) : diffuse(diffuse) { type = "diffuse"; }

	Color BRDF(Vector3 v, Vector3 l, Vector3 n) override
	{
		return diffuse * (1.0 / 3.14);
	}
	Color getBaseColor() override
	{
		return diffuse;
	}
	Ray sampleBRDF(Vector3 n, Vector3 v, Vector3 position, std::default_random_engine &generator, std::uniform_real_distribution<float> &distribution) override
	{
		//lambert cosine weighted
		float u = distribution(generator);				  //[0,1]
		float theta = distribution(generator) * 2 * 3.14; //[0,2pi]

		const float r = std::sqrt(u);
		const float x = r * std::cos(theta);
		const float y = r * std::sin(theta);
		Vector3 localV = Vector3(x, y, std::sqrt(std::max(0.0f, 1 - u)));
		Vector3 l;
		Vector3 axis = Vector3::cross(Vector3(0, 0, 1), n);
		if (axis == Vector3())
			l = localV * n.z;
		else
		{
			float rotAngle = std::acos(Vector3::dot(n, Vector3(0, 0, 1)));
			l = Quaternion::rotate(rotAngle, axis, localV);
		}
		return Ray(l.normalize(), position, 3.14 / (std::max(Vector3::dot(l, n), 0.0f)));
	}
};

class Mat_GGX : public Material
{
public:
	Color diffuse;
	float alpha, alphaSqr;

	Mat_GGX(Color diffuse, float alpha)
	{
		type = "glossy";
		this->diffuse = diffuse;
		alphaSqr = alpha * alpha;
	}

	Color getBaseColor() override
	{
		return diffuse;
	}
	Color BRDF(Vector3 v, Vector3 l, Vector3 n) override
	{
		//----------------------||
		//        vectors
		//----------------------||
		Vector3 h = (l + v).normalize();
		//----------------------||
		//  Specular Component
		//----------------------||
		//float alphaSqr = alpha * alpha;
		float D = alphaSqr /
				  (3.14 * std::pow(std::pow(std::max(Vector3::dot(n, h), 0.0f), 2) * (alphaSqr - 1) + 1, 2));

		float NdotV = std::max(Vector3::dot(n, v), 0.0f);
		float k = alpha * std::sqrt(2 / 3.14);
		float G1 = NdotV / (NdotV * (1 - k) + k);

		float NdotL = std::max(Vector3::dot(n, l), 0.0f);
		float G2 = NdotL / (NdotL * (1 - k) + k);
		float G = G1 * G2;
		float F0 = 0.04f;
		float F =
			F0 + (1 - F0) * std::pow(1.0f - std::max(Vector3::dot(h, v), 0.0f), 5.0f); // c'est n --> h

		float specular =
			(F * G * D) * (1.0f /
						   std::max(4.0f * std::max(Vector3::dot(v, n), 0.0f) * std::max(Vector3::dot(l, n), 0.0f), 0.001f));

		//----------------------||
		//   Diffuse Component
		//----------------------||
		Color result = (Color(1.0f, 1.0f, 1.0f) * specular);

		return result;
	}

	Ray sampleBRDF(Vector3 n, Vector3 v, Vector3 position, std::default_random_engine &generator, std::uniform_real_distribution<float> &distribution) override
	{
		//GGX
		//float alphaSqr = alpha * alpha;
		float u = distribution(generator);				//[0,1]
		float phi = distribution(generator) * 2 * 3.14; //[0,2pi]

		float theta = std::acos(std::sqrt((1.0f - u) /
										  ((alphaSqr - 1.0f) * u + 1.0f)));

		float st = std::sin(theta);
		Vector3 distribNormloc = Vector3(st * cos(phi), st * std::sin(phi), std::cos(theta));
		Vector3 distribNormGlob;
		Vector3 axis = Vector3::cross(Vector3(0, 0, 1), n);
		if (axis == Vector3())
			distribNormGlob = distribNormloc * n.z;
		else
		{
			float rotAngle = std::acos(Vector3::dot(n, Vector3(0, 0, 1)));
			distribNormGlob = Quaternion::rotate(rotAngle, axis, distribNormloc);
		}

		Vector3 l = distribNormGlob * 2.0f * Vector3::dot(v, distribNormGlob) - v;
		float D = (alpha * alpha) /
				  (3.14 * std::pow(std::pow(std::max(Vector3::dot(n, distribNormGlob), 0.0f), 2) * (alphaSqr - 1) + 1, 2));
		return Ray(l * (-1), position, (4.0f * std::abs(Vector3::dot(distribNormGlob, v))) / (D * Vector3::dot(distribNormGlob, n)));
	}
};

class Mat_Emissive : public Material
{
public:
	Color diffuse;
	float power;

	Mat_Emissive(Color diffuse, float power)
	{
		type = "emissive";
		this->diffuse = diffuse;
		this->power = power;
	}

	Color getBaseColor() override
	{
		return diffuse;
	}
	Color BRDF(Vector3 v, Vector3 l, Vector3 n) override
	{
		return diffuse * power;
	}
	Ray sampleBRDF(Vector3 n, Vector3 v, Vector3 position, std::default_random_engine &generator, std::uniform_real_distribution<float> &distribution) override
	{
		return Ray();
	}
};

#endif /* MATERIAL_HPP_ */
