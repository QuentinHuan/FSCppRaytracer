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

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void processInput(GLFWwindow *window);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

GLFWwindow *window;
GLCamera cam;

float deltaTime, lastFrame;

double lastX, lastY;
bool firstMouse = true;

//light Settings
glm::vec3 lPosition = glm::vec3(1.2f, 1.0f, 2.0f);
glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);

//mesh settings
glm::vec4 meshColor = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);

class GLContext
{
private:
public:
    GLContext()
    {
        printf("Start GL Viewer");
        mainGl();
    }

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
        window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);

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

        // tell GLFW to capture our mouse
        //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        // glad: load all OpenGL function pointers
        // ---------------------------------------
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cout << "Failed to initialize GLAD" << std::endl;
            return -1;
        }
    }

    void load()
    {
    }

    void render()
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glClearColor(0.05f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void dealocate()
    {
    }

    int mainGl()
    {
        init();
        //MESH LOADING
        mesh m = mesh("objectFiles/Monkey.obj");

        mesh light = mesh("objectFiles/Cube.obj");
        //Shader Compiling
        Shader lambertShader("src/OpenGl/shaders/vertex.vs", "src/OpenGl/shaders/frag.fs");
        Shader lightShader("src/OpenGl/shaders/vertex.vs", "src/OpenGl/shaders/fragLight.fs");
        //GLCamera
        cam = GLCamera(glm::vec3(0.0f, 0.0f, 2.0f));

        glEnable(GL_DEPTH_TEST);
        // render loop
        while (!glfwWindowShouldClose(window))
        {
            // input
            processInput(window);
            // render
            render();

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
            glfwPollEvents();
        }

        dealocate();
        glfwTerminate();
        return 0;
    }
};

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