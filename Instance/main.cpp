/*
	实例化
*/

#define STB_IMAGE_IMPLEMENTATION
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <stb_image.h>
#include <Shader.hpp>
#include <Camera.h>

using namespace std;
using namespace glm;

const unsigned int SCREEN_WIDTH = 800;
const unsigned int SCREEN_HEIGHT = 600;

void GLFWInit();
void frameBuffer_size_callBack(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void Draw();
void Render();
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xOffset, double yOffset);
void Clean();

/// <summary>
/// general
/// </summary>
GLFWwindow* window;
Camera camera(vec3(0.0F, 0.0F, 3.0F));

float lastX = 400, lastY = 300;
bool isFirstMouse = true;

float deltaTime = 0.0F;
float lastFrame = 0.0F;

unsigned int quadVAO;
unsigned int quadVBO;

int main()
{
	GLFWInit();

	Draw();

	Render();

	glfwTerminate();

	Clean();

	return 0;
}

/// <summary>
/// general
/// </summary>
void GLFWInit() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL) {
		cout << "Failed to create GLFW window" << endl;
		glfwTerminate();
		exit(-1);
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		cout << "Failed to initialize GLAD" << endl;
		exit(-1);
	}

	glViewport(0, 0, 800, 600);

	glfwSetFramebufferSizeCallback(window, frameBuffer_size_callBack);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwSetCursorPosCallback(window, mouse_callback);

	glfwSetScrollCallback(window, scroll_callback);
}

void processInput(GLFWwindow* window) {

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		//cameraPos += cameraSpeed * cameraFront;
		camera.ProcessKeyboard(FORWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		//cameraPos -= cameraSpeed * cameraFront;
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		//cameraPos -= normalize(cross(cameraFront, cameraUp)) * cameraSpeed;
		camera.ProcessKeyboard(LEFT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		//cameraPos += normalize(cross(cameraFront, cameraUp)) * cameraSpeed;
		camera.ProcessKeyboard(RIGHT, deltaTime);
	}
}


void frameBuffer_size_callBack(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	if (isFirstMouse) {
		lastX = xpos;
		lastY = ypos;
		isFirstMouse = false;
	}

	float xOffset = xpos - lastX;
	float yOffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;
	camera.ProcessMouseMovement(xOffset, yOffset);
}

void scroll_callback(GLFWwindow* window, double xOffset, double yOffset) {
	camera.ProcessMouseScroll(yOffset);
}


void Draw() {
	
}

void Render() {
	float quadVertices[] = {
		// 位置          // 颜色
		-0.05f,  0.05f,  1.0f, 0.0f, 0.0f,
		 0.05f, -0.05f,  0.0f, 1.0f, 0.0f,
		-0.05f, -0.05f,  0.0f, 0.0f, 1.0f,

		-0.05f,  0.05f,  1.0f, 0.0f, 0.0f,
		 0.05f, -0.05f,  0.0f, 1.0f, 0.0f,
		 0.05f,  0.05f,  0.0f, 1.0f, 1.0f
	};


	glm::vec2 translations[100];
	int index = 0;
	float offset = 0.1f;
	for (int y = -10; y < 10; y += 2)
	{
		for (int x = -10; x < 10; x += 2)
		{
			glm::vec2 translation;
			translation.x = (float)x / 10.0f + offset;
			translation.y = (float)y / 10.0f + offset;
			translations[index++] = translation;
		}
	}


	glGenVertexArrays(1, &quadVAO);

	// store instance data in an array buffer
	// --------------------------------------
	unsigned int instanceVBO;
	glGenBuffers(1, &instanceVBO);
	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * 100, &translations[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));


	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO); // this attribute comes from a different vertex buffer
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glVertexAttribDivisor(2, 1); // tell OpenGL this is an instanced vertex attribute.

	glBindVertexArray(0);

	Shader shader("Res/Shader/test1.vert", "Res/Shader/test1.frag");
	//shader.use();
	//for (unsigned int i = 0; i < 100; i++)
	//{
	//	stringstream ss;
	//	string index;
	//	ss << i;
	//	index = ss.str();
	//	shader.setVec2(("offsets[" + index + "]").c_str(), translations[i]);
	//}

	while (!glfwWindowShouldClose(window)) {
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		//input
		processInput(window);
		//render
		glClearColor(0.1F, 0.1F, 0.1F, 1.0F);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.use();
		glBindVertexArray(quadVAO);
		glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 100);
		glBindVertexArray(0);

		//check and call event,swap buffer
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

void Clean() {
	glDeleteVertexArrays(1, &quadVAO);
	glDeleteBuffers(1, &quadVBO);
}