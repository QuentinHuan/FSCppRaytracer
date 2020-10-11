
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
#include "BVH.hpp"
using namespace std;

int resX = 512, resY = resX;
int spp = 100;
int maxBounce = 4;
int maxBSPDepth = 2;

bool bMultiCore = 1;

int progress[4] = {0, 0, 0, 0};
const int progressEnd[4] = {spp, spp, spp, spp};

std::vector<Object> objList;
std::vector<Triangle> lights;
Camera cam(Vector3(0, -2.0, 0), resX, resY, 3.14 / 4.0, 1);
BVH bvh;

std::string pathToFiles = "/home/huan/git/FSCppRaytracer/objectFiles/";

void singleCore(Engine &e, Image &output)
{
	//Engine declaration
	Image oneSampleImg(resX, resY);
	Image imgFinal(resX, resY);

	//--------------------------------------------
	//Main Loop

	//cache building:
	//for each pixel
	for (int n = 1; n <= spp; n++) //for each sample
	{
		int counter = 0;

		//for each pixel
		for (int i = 0; i < resX * resY; i++)
		{
			oneSampleImg.array.at(i) = e.render(i);

			if (i == 0)
			{
				cout << "[";
			}
			if (((100 * i) / ((float)resX * resY)) > 10 * counter)
			{
				cout << "|";
				std::cout.flush();
				counter++;
			}
			if (i == resX * resY - 1)
			{
				cout << "]" << std::endl;
			}
		}

		for (int i = 0; i < resX; i++)
		{
			for (int j = 0; j < resY; j++)
			{
				imgFinal.array.at(i * resX + j) = ((imgFinal.array.at(i * resX + j) * (n - 1.f)) + oneSampleImg.array.at(i * resX + j)) * (float)(1.0 / (n));
			}
		}
		//imgFinal.exportPPM("img.ppm",8);
		cout << n << "/" << spp << "spp" << std::endl;
	}
	//PLACE HOLDER
	//--------------------------------------------
}

void fragment(int id, Engine &e, Image &output)
{
	Timer t{};
	//Engine declaration
	int dim = resX / 2;
	Image oneSampleImg(dim, dim);
	int offsetX, offsetY;
	//--------------------------------------------
	//Main Loop

	//cache building:
	//for each pixel
	for (int n = 1; n <= spp; n++) //for each sample
	{
		int imageSize = dim * dim;
		//for each pixel
		for (int i = 0; i < dim; i++)
		{
			for (int j = 0; j < dim; j++)
			{

				switch (id)
				{
				case 0:
					offsetY = 0;
					offsetX = 0;
					break;
				case 1:
					offsetY = dim;
					offsetX = 0;
					break;
				case 2:
					offsetY = 0;
					offsetX = dim;
					break;
				case 3:
					offsetY = dim;
					offsetX = dim;
					break;
				default:
					break;
				}
				oneSampleImg.array.at(i * dim + j) = e.render(((i + offsetX) * resX) + j + offsetY);
			}
		}
		for (int i = 0; i < dim; i++)
		{
			for (int j = 0; j < dim; j++)
			{
				output.array.at(((i + offsetX) * resX) + j + offsetY) = ((output.array.at(((i + offsetX) * resX) + j + offsetY) * (n - 1.f)) + oneSampleImg.array.at(i * dim + j)) * (float)(1.0 / (n));
			}
		}
		progress[id] = n;
	}
	return;
}

void display()
{
	Timer t{};
	bool quit = false;

	while (!quit)
	{
		if (t.elapsed() >= 0.1)
		{
			for (int k = 0; k < 3; k++)
			{
				printf("\n\n\n\n\n");
			}
			for (int i = 0; i < 4; i++)
			{
				printf("Thread %d --> [", i);
				for (int k = 0; k < (10 * progress[i]) / ((float)spp); k++)
				{
					printf("|");
				}
				for (int k = (10 * progress[i]) / ((float)spp); k < 10; k++)
				{
					printf(" ");
				}
				printf("]\n");
				//printf("Thread %d --> %d / %d \n",i,progress[i],spp);
			}

			int s = 0;
			for (int k = 0; k < 4; k++)
			{
				if (progress[k] == spp)
				{
					s = s + 1;
				}
			}
			if (s == 4)
			{
				quit = true;
			}

			fflush(stdout);
			t.reset();
		}
	}
	return;
}

int main()
{

	//scene Data
	Timer t{};

	objList.push_back(Object(pathToFiles + "Cornell box_Monkey.obj"));
	printf("object Import Done\n");
	//objList.push_back(Object("Cornell.obj"));
	//objList.push_back(Object("Furnace.obj"));
	//objList.push_back(Object("FurnaceHD.obj"));
	//objList.push_back(Object("Grid.obj"));

	for (auto it = objList.at(0).faces.begin(); it != objList.at(0).faces.end(); it++)
	{
		if (objList.at(0).mat.at(it->material).emission)
		{
			lights.push_back(*it);
		}
	}

	bvh = BVH(&objList.at(0).faces);
	printf("BVH Done\n");
	printf("tri count : %d / %d\n", bvh.triangleAmount(&bvh.NodeList.front()), objList.at(0).faces.size());
	Engine engine(resX, resY, cam, objList.at(0), lights, maxBounce, bvh);

	Image imgFinal(resX, resY);
	printf("Engine init done\n");
	printf("start computing\n");
	if (bMultiCore)
	{
		std::thread T0(fragment, 0, std::ref(engine), std::ref(imgFinal));
		std::thread T1(fragment, 1, std::ref(engine), std::ref(imgFinal));
		std::thread T2(fragment, 2, std::ref(engine), std::ref(imgFinal));
		std::thread T3(fragment, 3, std::ref(engine), std::ref(imgFinal));

		std::thread T4(display);

		T0.join();
		T1.join();
		T2.join();
		T3.join();
		T4.join();
	}
	else
	{
		singleCore(std::ref(engine), std::ref(imgFinal));
	}

	printf("----------------------\n");
	printf("DONE\n");
	printf("----------------------\n\n\n");
	printf("runtime : %f\n", t.elapsed());
	printf("\n\n");

	imgFinal.exportPPM("img.ppm", 8);

	return 0;
}
