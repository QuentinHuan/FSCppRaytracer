/*
 * Material.cpp
 *
 *  Created on: Sep 14, 2019
 *      Author: huan
 */

#include "Material.hpp"

Material::Material() : diffuse(Color()), emission(false), emissionPower(0){}

Material::Material(Color diffuse) : diffuse(diffuse), emission(false), emissionPower(0) {}

Material::Material(Color diffuse, bool emission, float emissionPower) : diffuse(diffuse), emission(emission), emissionPower(emissionPower){}

float Material::brdf(Vector3 n, Vector3 wr, Vector3 wi)
{
    //oren Nayar
    wr = (wr*(-1)).normalize();
    wi = (wi*(-1)).normalize();
    float sigmaSquare = sigma*sigma;
    float A = 1.f - 0.5*(sigmaSquare/(sigmaSquare+0.33));
    float B = 0.45*(sigmaSquare/(sigmaSquare+0.09));

    float thetai = std::acos(Vector3::dot(n,wi));
    float thetar = std::acos(Vector3::dot(n,wr));

    wi=wi-n*(Vector3::dot(n,wi));
    wr=wr-n*(Vector3::dot(n,wr));

    float deltaPhi=std::acoshf(Vector3::dot(wr,wi)); //phiR - phiI

    return std::abs((1/3.14)*(A + (1*B*std::max(0.f,std::cos(deltaPhi))*std::sin(std::max(thetai,thetar))*std::tan(std::min(thetai,thetar)))));
    //return (1/3.14);
}