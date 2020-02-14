/*
 * Camera.cpp
 *
 *  Created on: Sep 1, 2019
 *      Author: huan
 */

#include "Camera.hpp"

Camera::Camera() {
	// TODO Auto-generated constructor stub
	this->position = Vector3 {0,0,0};
	this->resX = 0;
	this->resY = 0;
	this->depthOfField = 0;

	size = 1;
}

Camera::Camera(Vector3 position,int resX,int resY, float depthOfField, float size) {
	this->position = position;
	this->resX = resX;
	this->resY = resY;
	this->depthOfField = depthOfField;
	this->size = size;
	calcFocal();
}

void Camera::calcFocal() {
	focal.x = position.x;
	focal.y = position.y - (size/(2*tan(depthOfField/2)));
	focal.z = position.z;

}

Ray Camera::camRay(int pixelX, int pixelY) {


	//dimension de la cam
	//pas terrible... a revoir
	float pasX = size / resX;
	float pasY = pasX;
	Vector3 pixelCoord = Vector3(pixelX*pasX - size/2,0,pixelY*pasY - size/2);
	Ray r ( (position + pixelCoord) - focal, focal);
	//r.dir = Vector3(0,1,0);
	//r.pos = position + pixelCoord;

	return r;
}
