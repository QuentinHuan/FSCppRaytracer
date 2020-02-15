
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
int maxBounce = 2;
int maxBSPDepth = 2;

int main() {

	Timer t{};


	//scene Data
	std::vector<Object> objList;
	std::vector<Triangle> lights;
	objList.push_back(Object("Cornell box.obj"));
	//objList.push_back(Object("Cornell.obj"));
	//objList.push_back(Object("Furnace.obj"));
	//objList.push_back(Object("FurnaceHD.obj"));
	//objList.push_back(Object("Grid.obj"));

	Camera cam(Vector3(-0.4,-2,0), resX,resY,3.14/4.0,1);

	//Engine declaration

	BVH bvh(objList.at(0).faces);

	Image oneSampleImg(resX,resY);
	Image imgFinal(resX,resY);
	Statistics statCounter{};


	for(auto it=objList.at(0).faces.begin(); it != objList.at(0).faces.end(); it++)
	{
		if(it->material.emission)
		{
			lights.push_back(*it);
		}
	}

	Engine engine(objList.at(0).faces,lights, statCounter,maxBounce,bvh);

	vector<HitInfo> cache;
	cache.reserve(resX*resY);

	vector<float> f = vector<float>();
	f.push_back(1);f.push_back(2);f.push_back(3);

	vector<float> f2 = vector<float>();
	f2.push_back(4);f2.push_back(5);f2.push_back(6);

	f.reserve( f.size() + f2.size());
	f.insert( f.end(), f2.begin(), f2.end());


	vector<Triangle> T = vector<Triangle>();
	T.push_back(objList.at(0).faces.at(0));



	double debugT = 0;
	//--------------------------------------------
	//Main Loop

	//cache building:
	//for each pixel

	cout  << "bvh build done in " << t.elapsed() << "sec" << std::endl;

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
		Timer debug{};


		for(int i=0;i<resX;i++)
		{
			for(int j=0;j<resY;j++)
			{
				int pixelIndex = i*resX+j;
				Ray r = cam.camRay(i,j);


				Color pixel = engine.rayTrace(r, cache.at(pixelIndex));


				oneSampleImg.array.at(pixelIndex) = pixel;

			}


			debugT += debug.elapsed();


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
	cout << statCounter.toString(true) << endl;
	//cout << "debug = " << statCounter.criteria.at(1) << endl;



	printf("DONE");
	return 0;
}


