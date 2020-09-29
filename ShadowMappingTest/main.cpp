/*
	“ı”∞Ã˘Õº≤‚ ‘
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

//cube
unsigned int vao_cube;
unsigned int vbo_cube;
Shader* shader_cube;
unsigned int ubo_mat_cube;

//light 
vec3 pos_light = vec3(0, 5, 5);

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
	///////////////////
	//cube

	shader_cube = new Shader("Shader/cube.vert", "Shader/cube.frag");

	float cubeVertices[] = {
		// positions          // texture Coords
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	glGenVertexArrays(1, &vao_cube);
	glGenBuffers(1, &vbo_cube);

	glBindVertexArray(vao_cube);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_cube);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	unsigned int uniformBlockIndexCube = glGetUniformBlockIndex(shader_cube->ID, "Matrices");
	glUniformBlockBinding(shader_cube->ID, uniformBlockIndexCube, 0);
	glGenBuffers(1, &ubo_mat_cube);

	glBindBuffer(GL_UNIFORM_BUFFER, ubo_mat_cube);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(mat4), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	glBindBufferRange(GL_UNIFORM_BUFFER, 0, ubo_mat_cube, 0, 2 * sizeof(mat4));

	mat4 projection = perspective(radians(45.F), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1F, 100.F);
	glBindBuffer(GL_UNIFORM_BUFFER, ubo_mat_cube);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(mat4), value_ptr(projection));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Render() {


	glEnable(GL_DEPTH_TEST);
	while (!glfwWindowShouldClose(window)) {
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		//input
		processInput(window);
		//render
		glClearColor(0.1F, 0.1F, 0.1F, 1.0F);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glBindVertexArray(vao_cube);
		mat4 model = mat4(1.0F);
		mat4 view = camera.GetViewMatrix();
		glBindBuffer(GL_UNIFORM_BUFFER, ubo_mat_cube);
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(mat4), sizeof(mat4), value_ptr(view));
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		glBindVertexArray(vao_cube);
		shader_cube->use();

		// the cube to receive shadow 
		model = translate(model, vec3(-0.6, 0, 0));
		shader_cube->setMat4("model", model);
		shader_cube->setVec4("fColor", vec4(46.F / 255.F, 139.F / 255.F, 87.F / 255.F, 1));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = mat4(1.0F);
		model = translate(model, vec3(0.6, 0, 0));
		shader_cube->setMat4("model", model);
		shader_cube->setVec4("fColor", vec4(83.F / 255.F, 134.F / 255.F, 139.F / 255.F, 1));
		glDrawArrays(GL_TRIANGLES, 0, 36);


		// the cube to mask
		model = mat4(1.0F);
		model = translate(model, vec3(0, 2, 2));
		model = scale(model, vec3(0.5F, 0.5F, 0.5F));
		shader_cube->setMat4("model", model);
		shader_cube->setVec4("fColor", vec4(1, 1, 0, 1));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// light
		model = mat4(1.0F);
		model = translate(model, pos_light);
		model = scale(model, vec3(0.1F, 0.1F, 0.1F));
		shader_cube->setMat4("model", model);
		shader_cube->setVec4("fColor", vec4(1, 1, 1, 1));
		glDrawArrays(GL_TRIANGLES, 0, 36);


		//check and call event,swap buffer
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

void Clean() {
	glDeleteVertexArrays(1, &vao_cube);
	glDeleteBuffers(1, &vbo_cube);
}