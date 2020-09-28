
//============================================================================
// Name        : Raytracer_MKII.cpp
// Author      : Quentin Huan
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================
//raytracer includes
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

//GL includes
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <shader.h>
#include <iostream>
#include "mesh.h"
#include "GLCamera.h"

//--------------------------------------------
//			Raytracing Interface
//--------------------------------------------
int resX = 128, resY = resX;
int spp = 10;
int maxBounce = 4;
int maxBSPDepth = 2;

int progress[4] = {0, 0, 0, 0};
const int progressEnd[4] = {spp, spp, spp, spp};

std::vector<Object> objList;
std::vector<Triangle> lights;
Camera camera(Vector3(-0.4, -2, 0), resX, resY, 3.14 / 4.0, 1);
BVH bvh;

std::string pathToFiles = "/home/huan/git/FSCppRaytracer/objectFiles/";

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

int render(float *output)
{

	//scene Data
	Timer t{};

	objList.push_back(Object(pathToFiles + "CornellBox.obj"));
	printf("object Import Done\n");

	for (auto it = objList.at(0).faces.begin(); it != objList.at(0).faces.end(); it++)
	{
		if (it->material.emission)
		{
			lights.push_back(*it);
		}
	}

	bvh = BVH(&objList.at(0).faces);
	printf("BVH Done\n");
	printf("tri count : %d / %d\n", bvh.triangleAmount(&bvh.NodeList.front()), objList.at(0).faces.size());
	Statistics statCounter{};
	Engine engine(resX, resY, camera, objList.at(0).faces, lights, statCounter, maxBounce, bvh);

	Image imgFinal(resX, resY);
	printf("Engine init done\n");
	printf("start computing\n");

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

	//statCounter.runtime = t.elapsed();

	printf("----------------------\n");
	printf("DONE\n");
	printf("----------------------\n\n\n");
	printf("runtime : %f\n", t.elapsed());
	printf("\n\n");

	//	imgFinal.exportPPM("img.ppm", 8);

	const int size = resX * resY * 3;

	for (int i = 0; i < resX * resY; i++)
	{
		output[i * 3 + 0] = imgFinal.array.at(i).r;
		output[i * 3 + 1] = imgFinal.array.at(i).g;
		output[i * 3 + 2] = imgFinal.array.at(i).b;
	}

	return 0;
}

//--------------------------------------------
//				OpenGl Viewer
//--------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void processInput(GLFWwindow *window);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

// settings
const unsigned int SCR_WIDTH = 1024;
const unsigned int SCR_HEIGHT = 1024;

GLFWwindow *window;
GLFWwindow *window2;
GLCamera cam;

float deltaTime, lastFrame;

double lastX, lastY;
bool firstMouse = true;

//light Settings
glm::vec3 lPosition = glm::vec3(1.2f, 1.0f, 2.0f);
glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);

//mesh settings
glm::vec4 meshColor = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);

int init()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// --------------------
	window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Viewport", NULL, NULL);
	window2 = glfwCreateWindow(resX, resY, "Render", NULL, window);

	glfwSetWindowPos(window, 0, 0);
	glfwSetWindowPos(window2, 0 + SCR_WIDTH + 10, 0);

	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	//callbacks
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	glfwSetFramebufferSizeCallback(window2, framebuffer_size_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwRequestWindowAttention(window);
	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
}

int mainGl(float *renderedImg)
{
	init();
	//MESH LOADING
	mesh m = mesh("objectFiles/Monkey.obj");
	mesh light = mesh("objectFiles/Cube.obj");
	mesh plane = mesh("objectFiles/plane.obj");
	//Shader Compiling
	Shader lambertShader("src/OpenGl/shaders/vertex.vs", "src/OpenGl/shaders/frag.fs");
	Shader lightShader("src/OpenGl/shaders/vertex.vs", "src/OpenGl/shaders/fragLight.fs");
	Shader imgViewerShader("src/OpenGl/shaders/vertex.vs", "src/OpenGl/shaders/texture.fs");
	//GLCamera
	cam = GLCamera(glm::vec3(0.0f, 0.0f, 2.0f));

	glEnable(GL_DEPTH_TEST);
	// render loop
	while (!glfwWindowShouldClose(window) && !glfwWindowShouldClose(window2))
	{
		glfwMakeContextCurrent(window);
		// input
		processInput(window);

		// render
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glClearColor(0.05f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//transformation
		//create view and perspective matrices
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);
		view = cam.GetViewMatrix();
		projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

		lightShader.use();
		//light obj transformation
		glBindVertexArray(light.VAO);
		glm::mat4 lmodel = glm::mat4(1.0f);
		lmodel = glm::rotate(lmodel, float(glm::sin(glfwGetTime())) * glm::radians(90.0f), glm::vec3(1.0f, 0.3f, 0.5f));
		lmodel = glm::translate(lmodel, lPosition);
		lmodel = glm::scale(lmodel, glm::vec3(0.1, 0.1, 0.1));

		lightShader.setMat4("view", view);
		lightShader.setMat4("projection", projection);
		lightShader.setMat4("model", lmodel);
		lightShader.setVec3("lColor", lightColor);
		glDrawElements(GL_TRIANGLES, light.objData.indicesSize / sizeof(int), GL_UNSIGNED_INT, 0);

		lambertShader.use();
		//model tranformation
		glBindVertexArray(m.VAO);
		glm::mat4 model = glm::mat4(1.0f);
		float angle = 20.0f;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
		model = glm::scale(model, glm::vec3(0.5, 0.5, 0.5));

		lambertShader.setMat4("view", view);
		lambertShader.setMat4("projection", projection);
		lambertShader.setMat4("model", model);
		lambertShader.setVec3("lColor", lightColor);

		lambertShader.setVec3("material.ambient", 1.0f, 0.5f, 0.31f);
		lambertShader.setVec3("material.diffuse", 1.0f, 0.5f, 0.31f);
		lambertShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
		lambertShader.setFloat("material.shininess", 32.0f);

		lambertShader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
		lambertShader.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f); // darken diffuse light a bit
		lambertShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

		lambertShader.setVec3("lightPos", lmodel * glm::vec4(lPosition, 1.0f));
		lambertShader.setVec3("viewPos", cam.Position);
		glDrawElements(GL_TRIANGLES, m.objData.indicesSize / sizeof(int), GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);

		glfwMakeContextCurrent(window2);
		glClearColor(0.45f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//upload to GPU texture
		GLuint tex;
		glGenTextures(1, &tex);
		glBindTexture(GL_TEXTURE_2D, tex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, resX, resY, 0, GL_RGB, GL_UNSIGNED_BYTE, renderedImg);
		glGenerateMipmap(GL_TEXTURE_2D);

		glDrawElements(GL_TRIANGLES, plane.objData.indicesSize / sizeof(int), GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window2);

		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cam.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cam.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cam.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cam.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	cam.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
	cam.ProcessMouseScroll(yoffset);
}

int main()
{
	float img[resY * resX * 3];
	//render(&img[0]);

	for (int i = 0; i < resY * resX * 3; i++)
	{
		if (i % 10 == 0)
		{
			img[i] = 1.0;
		}
	}

	mainGl(&img[0]);
}