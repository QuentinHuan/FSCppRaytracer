
//============================================================================
// Name        : Raytracer_MKII.cpp
// Author      : Quentin Huan
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "Object.hpp"
#include "Image.hpp"
#include "Camera.hpp"
#include "Color.hpp"
#include "Material.hpp"
#include <ctime>


#include "Engine.hpp"
#include "Statistics.hpp"
#include "BVH.hpp"
using namespace std;

int resX = 256, resY=resX;
int spp = 10;
int maxBounce = 4;
int maxBSPDepth = 2;

int main() {

	Timer t{};

	//scene Data
	std::vector<Object> objList;
	objList.push_back(Object("Cornell box.obj"));
	//objList.push_back(Object("Cornell.obj"));
	//objList.push_back(Object("Furnace.obj"));
	//objList.push_back(Object("FurnaceHD.obj"));
	//objList.push_back(Object("Grid.obj"));

	Camera cam(Vector3(-0.4,-2,0), resX,resY,3.14/4.0,1);

	//Engine declaration
	Image oneSampleImg(resX,resY);
	Image imgFinal(resX,resY);
	Statistics statCounter{};
	Engine engine(objList, statCounter,maxBounce);

	vector<HitInfo> cache;
	cache.reserve(resX*resY);

	BVH bvh(objList.at(0).faces);

	//--------------------------------------------
	//Main Loop

	//cache building:
	//for each pixel

	//cout  << bvh.tree->toString();

	int counter = 0;
	for(int i=0;i<resX;i++)
	{

		for(int j=0;j<resY;j++)
		{
			Ray r = cam.camRay(i,j);
			cache.push_back(engine.buildCache(r));
		}

		if(i==0)
		{
			cout  << "[";
		}
		//cout  << (100.0*i)/(float)resX;
		if(((100.0*i)/(float)resX) > 5*counter)
		{
			cout <<"|";
			std::cout.flush();
			counter++;
		}
		if(i==resX-1)
		{
			cout <<"]" << std::endl;
		}
	}
	cout  <<"Cache Building done" << std::endl;


	for(int n=1;n<=spp;n++)//for each sample
	{
		int counter=0;
		//for each pixel
		for(int i=0;i<resX;i++)
		{
			for(int j=0;j<resY;j++)
			{
				int pixelIndex = i*resX+j;
				Ray r = cam.camRay(i,j);
				Color pixel = Color(0,0,0);

				pixel = pixel + engine.rayTrace(r, cache.at(pixelIndex));
				oneSampleImg.array.at(pixelIndex) = pixel;
			}

			if(i==0)
			{
				cout  << "[";
			}
			//cout  << (100.0*i)/(float)resX;
			if(((100.0*i)/(float)resX) > 5*counter)
			{
				cout <<"|";
				std::cout.flush();
				counter++;
			}
			if(i==resX-1)
			{
				cout <<"]" << std::endl;
			}

		}

		for(int i=0;i<resX;i++)
		{
			for(int j=0;j<resY;j++)
			{
				imgFinal.array.at(i*resX+j) = ((imgFinal.array.at(i*resX+j)*(n-1.f)) + oneSampleImg.array.at(i*resX+j))*(float)(1.0/(n));
			}
		}

		imgFinal.exportPPM("img.ppm",8);
		cout  << n << "/" << spp <<"spp" << std::endl;
	}
	//PLACE HOLDER
	//--------------------------------------------

	statCounter.runtime = t.elapsed();
	imgFinal.exportPPM("img.ppm",8);
	cout << statCounter.toString(false) << endl;




	printf("DONE");
	return 0;
}


