
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

#include <pthread.h>
#include <thread>
#include <future>

#include "Engine.hpp"
#include "Statistics.hpp"
#include "BVH.hpp"
using namespace std;

int resX = 512, resY=resX;
int spp = 10;
int maxBounce = 4;
int maxBSPDepth = 2;

bool bMultiCore = 1;

std::vector<Object> objList;
std::vector<Triangle> lights;
Camera cam(Vector3(-0.4,-2,0), resX,resY,3.14/4.0,1);
BVH bvh;

void singleCore(Engine &e,Image &output)
{


	//Engine declaration
	Image oneSampleImg(resX,resY);
	Image imgFinal(resX,resY);
	Statistics statCounter{};

	//--------------------------------------------
	//Main Loop

	//cache building:
	//for each pixel
	for(int n=1;n<=spp;n++)//for each sample
	{
		int counter=0;

		//for each pixel
		for(int i = 0; i <  resX*resY; i++)
		{
			oneSampleImg.array.at(i) = e.render(i);

			if(i==0)
			{
				cout  << "[";
			}
			if(((100*i)/((float)resX*resY)) > 10*counter)
			{
				cout <<"|";
				std::cout.flush();
				counter++;
			}
			if(i==resX*resY-1)
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
		//imgFinal.exportPPM("img.ppm",8);
		cout  << n << "/" << spp <<"spp" << std::endl;
	}
	//PLACE HOLDER
	//--------------------------------------------


}

void fragment(int id, Engine &e,Image &output)
{
	Timer t{};
	//Engine declaration
	int dim = resX/2;
	Image oneSampleImg(dim,dim);
	int offsetX,offsetY;
	//--------------------------------------------
	//Main Loop

	//cache building:
	//for each pixel
	for(int n=1;n<=spp;n++)//for each sample
	{
		int imageSize = dim*dim;
		//for each pixel
		for(int i = 0; i <  dim; i++)
		{
			for(int j = 0; j <  dim; j++)
			{

				switch (id) {
					case 0:
						offsetY = 0;
						offsetX=0;
						break;
					case 1:
						offsetY = dim;
						offsetX=0;
						break;
					case 2:
						offsetY = 0;
						offsetX=dim;
						break;
					case 3:
						offsetY = dim;
						offsetX=dim;
						break;
					default:
						break;
				}
				oneSampleImg.array.at(i*dim + j) = e.render(((i+offsetX)*resX)+ j + offsetY);
			}
		}
		//oneSampleImg.exportPPM("onespp.ppm",8);

		for(int i=0;i<dim;i++)
		{
			for(int j=0;j<dim;j++)
			{
				output.array.at(((i+offsetX)*resX)+ j + offsetY) = ((output.array.at(((i+offsetX)*resX)+ j + offsetY)*(n-1.f)) + oneSampleImg.array.at(i*dim+j))*(float)(1.0/(n));
			}
		}


		//imgFinal.exportPPM("img.ppm",8);
		if(id == 1)
		{
			cout  << "Thread " << id << "--> "<< n << "/" << spp <<"spp" << std::endl;
		}

	}
	//PLACE HOLDER
	//--------------------------------------------
	//return oneSampleImg;
	return;
}

int main() {

	//scene Data
	Timer t{};

	objList.push_back(Object("/home/huan/git/FSCppRaytracer/Raytracer_MKII/dragon.obj"));
	printf("object Import Done\n");
	//objList.push_back(Object("Cornell.obj"));
	//objList.push_back(Object("Furnace.obj"));
	//objList.push_back(Object("FurnaceHD.obj"));
	//objList.push_back(Object("Grid.obj"));

	for(auto it=objList.at(0).faces.begin(); it != objList.at(0).faces.end(); it++)
		{
			if(it->material.emission)
			{
				lights.push_back(*it);
			}
		}

	bvh = BVH(&objList.at(0).faces);
	printf("BVH Done\n");
	printf("tri count : %d / %d\n", bvh.triangleAmount(&bvh.NodeList.front()),objList.at(0).faces.size());
	Statistics statCounter{};
	Engine engine(resX,resY,cam,objList.at(0).faces,lights, statCounter,maxBounce,bvh);

	Image imgFinal(resX,resY);
	printf("Engine init done\n");
	printf("start computing\n");
	if(bMultiCore)
	{
		//std::future<Image> T0 = std::async(fragment,0);
		//std::future<Image> T1 = std::async(fragment,1);
		//std::future<Image> T2 = std::async(fragment,2);
		//std::future<Image> T3 = std::async(fragment,3);



		std::thread T0(fragment,0,std::ref(engine),std::ref(imgFinal));
		std::thread T1(fragment,1,std::ref(engine),std::ref(imgFinal));
		std::thread T2(fragment,2,std::ref(engine),std::ref(imgFinal));
		std::thread T3(fragment,3,std::ref(engine),std::ref(imgFinal));
		//Image I1 = T1.get();
		//Image I2 = T2.get();
		//Image I3 = T3.get();
		//fragment(0);
		T0.join();
		T1.join();
		T2.join();
		T3.join();
	}
	else
	{
		singleCore(std::ref(engine),std::ref(imgFinal));
	}

	statCounter.runtime = t.elapsed();
	cout << statCounter.toString(true) << endl;
	//cout << "debug = " << statCounter.criteria.at(1) << endl;

	imgFinal.exportPPM("img.ppm",8);
	printf("DONE");
	return 0;
}


