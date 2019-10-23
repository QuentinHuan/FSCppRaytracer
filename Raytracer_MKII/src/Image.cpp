/*
 * Image.cpp
 *
 *  Created on: Sep 1, 2019
 *      Author: huan
 */

#include "Image.hpp"
#include<cmath>

Image::Image() {
	// TODO Auto-generated constructor stub
	resX = 1;
	resY = 1;
	array.push_back(Color {0,0,0});
}

Image::Image(int resX, int resY) {
	this->resX = resX;
	this->resY = resY;
	array.assign(resX*resY,Color {0,0,0});
}

Image::Image(int resX, int resY, std::vector<Color> sourceArray) {
	this->resX = resX;
	this->resY = resY;
	array = sourceArray;
}



void Image::exportPPM(std::string file,int bitColorDepth) {
    std::ofstream fichier { file };

    struct colorInt{
    	int r,g,b;
    };

    std::vector<colorInt> img;
    int maxValue = pow(2,bitColorDepth)-1;

    for(auto i = 0; i<array.size();i++)
    {
    	colorInt c;
    	c.r = (int)(array.at(i).r*maxValue);
    	c.g = (int)(array.at(i).g*maxValue);
    	c.b = (int)(array.at(i).b*maxValue);
    	img.push_back(c);
    }


    //Header
    fichier << "P3" << std::endl;//magic number
    fichier << std::to_string(resX) + " " + std::to_string(resY) << std::endl;//resolution
    fichier << std::to_string(maxValue) << std::endl;//color depth

    //data
    for(int i = 0; i<resY;i++)//ligne
    {
		std::string ligne = "";
    	for(int j = i*resX; j<(i+1)*resX;j++)
    	{
    		ligne += std::to_string(img.at(j).r) + " " + std::to_string(img.at(j).g) + " " + std::to_string(img.at(j).b) + "  ";
    	}



    	fichier << ligne << std::endl;
    }

}
