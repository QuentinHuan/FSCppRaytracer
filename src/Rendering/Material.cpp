/*
 * Material.cpp
 *
 *  Created on: Sep 14, 2019
 *      Author: huan
 */

#include "Material.hpp"

Material::Material() : diffuse(Color()), emission(false), emissionPower(0) {}

Material::Material(Color diffuse, float alpha) : diffuse(diffuse), emission(false), emissionPower(0), alpha(alpha) {}

Material::Material(Color diffuse, bool emission, float emissionPower) : diffuse(diffuse), emission(emission), emissionPower(emissionPower) {}

Color Material::BRDF(Vector3 v, Vector3 l, Vector3 n)
{
    if (alpha == 1) //lambert
    {
        return this->diffuse * (1.0 / 3.14);
    }
    else //cook torrance
    {
        Color albedo = this->diffuse;
        //----------------------||
        //        vectors
        //----------------------||
        Vector3 h = (l + v).normalize();
        //Vector3 h = l.normalize();
        //----------------------||
        //  Specular Component
        //----------------------||
        float alphaSqr = std::pow(alpha, 2.0f);
        float metallic = 0.0f;

        float D = alphaSqr /
                  (3.14 * std::pow(std::pow(std::max(Vector3::dot(n, h), 0.0f), 2) * (alphaSqr - 1) + 1, 2));

        float NdotV = std::max(Vector3::dot(n, v), 0.0f);
        float k = alpha * std::sqrt(2 / 3.14); // direct light
        float G1 = NdotV / (NdotV * (1 - k) + k);

        float NdotL = std::max(Vector3::dot(n, l), 0.0f);
        float G2 = NdotL / (NdotL * (1 - k) + k);
        float G = G1 * G2;

        float F0 = 0.04f;
        //F0 = mix(F0, albedo, metallic); //matallic ratio
        float F =
            F0 + (1 - F0) * std::pow(1.0f - std::max(Vector3::dot(h, v), 0.0f), 5.0f); // c'est n --> h

        float specular =
            (F * G * D) * (1.0f /
                           std::max(4.0f * std::max(Vector3::dot(v, n), 0.0f) * std::max(Vector3::dot(l, n), 0.0f), 0.001f));

        //----------------------||
        //   Diffuse Component
        //----------------------||
        float kd = 1.0f - F;
        kd = kd * (1.0f - metallic);

        float NdotOmega = std::max(Vector3::dot(n, l), 0.0f);
        Color diffuse = albedo * kd * (1.0 / 3.14);

        Color result = (Color(1.0f, 1.0f, 1.0f) * specular);
        //return specular + 1 * kd * (1.0 / 3.14);
        return result;
        //lambert
        //return (1.0 / 3.14);
    }
}

Ray Material::sampleBRDF(Vector3 n, Vector3 v, Vector3 position, std::default_random_engine &generator, std::uniform_real_distribution<float> &distribution)
{
    if (alpha == 1)
    {
        //lambert cosine weighted
        float u = distribution(generator);                //[0,1]
        float theta = distribution(generator) * 2 * 3.14; //[0,2pi]

        const float r = std::sqrt(u);
        const float x = r * std::cos(theta);
        const float y = r * std::sin(theta);
        Vector3 localV = Vector3(x, y, std::sqrt(std::max(0.0f, 1 - u)));
        Vector3 l = Vector3();
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
    else
    {
        //GGX
        float u = distribution(generator);              //[0,1]
        float phi = distribution(generator) * 2 * 3.14; //[0,2pi]

        float theta = std::acos(std::sqrt((1.0f - u) /
                                          ((alpha * alpha - 1.0f) * u + 1.0f)));

        float st = std::sin(theta);
        float ct = std::cos(theta);
        Vector3 distribNormloc = Vector3(st * cos(phi), st * std::sin(phi), ct);
        //Vector3 localV = distribNorm * 2.0f * Vector3::dot(v, distribNorm) - v;
        //Vector3 localV = distribNorm;
        Vector3 distribNormGlob = Vector3();
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
                  (3.14 * std::pow(std::pow(std::max(Vector3::dot(n, distribNormGlob), 0.0f), 2) * (alpha * alpha - 1) + 1, 2));
        return Ray(l * (-1), position, (4.0f * std::abs(Vector3::dot(distribNormGlob, v))) / (D * Vector3::dot(distribNormGlob, n)));
    }
}