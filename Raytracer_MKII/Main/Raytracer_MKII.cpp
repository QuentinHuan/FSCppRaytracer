
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

#include "BSP.hpp"
#include "Engine.hpp"
#include "Statistics.hpp"
using namespace std;

int resX = 256, resY=resX;
int spp = 10;
int maxBounce = 4;
int maxBSPDepth = 9;

bool debug = 0;
bool useBSP = 0;
bool useCache = 0;

int main() {


	float sceneScale = 2;
	Vector3 offset;
	int sizeObj,sizePartition;

	Timer t{};

	//scene Data
	std::vector<Object> objList;
	objList.push_back(Object("Cornell box.obj"));
	//objList.push_back(Object("Furnace.obj"));
	//objList.push_back(Object("FurnaceHD.obj"));
	//objList.push_back(Object("Grid.obj"));
	sizeObj = objList.at(0).faces.size();

	Camera cam(Vector3(-0.4,-2,0), resX,resY,3.14/4.0,1);

	//acceleration structure
	sceneScale = objList.at(0).getScale()+0.001;
	offset = objList.at(0).getCenter();
	BSP tree = BSP(maxBSPDepth,objList.at(0).faces,sceneScale,offset);

	sizePartition = tree.debugPartition().size();

	if(debug)
	{
		objList.at(0).faces = tree.debugPartition();
		useBSP = false;
	}
	sizePartition = objList.at(0).faces.size();

	cout  <<"BSP Building done" << std::endl;

	//Engine declaration
	Image oneSampleImg(resX,resY);
	Image imgFinal(resX,resY);
	Statistics statCounter{};
	Engine engine(objList, statCounter,tree,maxBounce,useBSP,useCache);

	vector<HitInfo> cache;


	//--------------------------------------------
	//Main Loop

	//cache building:



	//for each pixel

	for(int i=0;i<resX;i++)
	{
		for(int j=0;j<resY;j++)
		{
			Ray r = cam.camRay(i,j);
			if(useCache)
			{
				cache.push_back(engine.buildCache(r));
			}
			else
			{
				cache.push_back(HitInfo());
			}

		}
		//cout  << i << "/" << resX << std::endl;
	}
	cout  <<"Cache Building done" << std::endl;



	for(int n=1;n<=spp;n++)//for each sample
	{
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

			//cout  << i << "/" << resX << std::endl;
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
