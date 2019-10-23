
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
#include "Engine.hpp"
#include "Color.hpp"
#include "Material.hpp"
#include "Statistics.hpp"
#include "BSP.hpp"
#include <ctime>
using namespace std;

int resX = 128, resY=resX;
int spp = 1000;
int maxBounce = 4;
int maxBSPDepth = 3;

bool debug = 0;
bool useBSP = 0;

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
	Engine engine(objList, statCounter,tree,maxBounce,useBSP);

	//--------------------------------------------
	//PLACE HOLDER

	for(int n=1;n<=spp;n++)
	{
		for(int i=0;i<resX;i++)
		{
			for(int j=0;j<resY;j++)
			{
				if(i == resX/2)
				{

				}
				Ray r = cam.camRay(i,j);
				HitInfo hit{};
				Color pixel = Color(0,0,0);

				pixel = pixel + engine.rayTrace(r);

				oneSampleImg.array.at(i*resX+j) = pixel;

			}

			//cout  << i << "/" << resX << std::endl;
			if(i==170)
			{
				cout  << "bug" << std::endl;
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
	cout << statCounter.toString() << endl;



	printf("DONE");
	return 0;
}
