/*
 * Camera.hpp
 *
 *  Created on: Sep 1, 2019
 *      Author: huan
 */
#pragma once
#ifndef CAMERA_HPP_
#define CAMERA_HPP_
#include "Image.hpp"
#include "Ray.hpp"

class Camera {
private:
	void calcFocal();

public:
	Vector3 position, focal;
	int resX,resY;
	float depthOfField, size;

	Camera();
	Camera(Vector3 position, int resX,int resY, float depthOfField,float size);
	Ray camRay(int pixelX,int pixelY);




};

#endif /* CAMERA_HPP_ */
