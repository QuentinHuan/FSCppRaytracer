/*
 * Color.cpp
 *
 *  Created on: Sep 1, 2019
 *      Author: huan
 */

#include "Color.hpp"

Color::Color() {
	// TODO Auto-generated constructor stub
	this->r = 0;
	this->g = 0;
	this->b = 0;
}

Color::Color(float r, float g, float b) {
	this->r = r;
	this->g = g;
	this->b = b;
	applyBounds();
}

void Color::applyBounds() {
	if(r > 1) r = 1;
	if(g > 1) g = 1;
	if(b > 1) b = 1;

	if(r < 0) r = 0;
	if(g < 0) g = 0;
	if(b < 0) b = 0;
	return;
}

Color Color::operator +(Color c) {
	Color result;
	result.r = this->r + c.r;
	result.g = this->g + c.g;
	result.b = this->b + c.b;
	applyBounds();
	return result;
}

Color Color::operator -(Color c) {
	Color result;
	result.r = this->r - c.r;
	result.g = this->g - c.g;
	result.b = this->b - c.b;
	applyBounds();
	return result;
}


Color Color::operator *(Color c) {
	Color result;
	result.r = this->r * c.r;
	result.g = this->g * c.g;
	result.b = this->b * c.b;
	applyBounds();
	return result;
}

Color Color::operator *(float f) {
	Color result;
	result.r = this->r * f;
	result.g = this->g * f;
	result.b = this->b * f;
	applyBounds();
	return result;
}

bool Color::operator ==(Color c) {
	if(r == c.r && b == c.b && g == c.g) return true;
	else return false;
}
