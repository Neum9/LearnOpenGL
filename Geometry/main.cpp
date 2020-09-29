#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <Shader.hpp>
#include <Camera.h>
#include <Model.h>;

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

unsigned int vao1;
unsigned int vbo1;


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

void Clean() {
	glDeleteVertexArrays(1, &vao1);
	glDeleteBuffers(1, &vbo1);
}

/////////////////////////////////////////////
// 下面是渲染模型爆炸
/////////////////////////////////////////////
void Draw() {



}
void Render() {
	Shader shader_model("Shader/vert2.vert", "Shader/frag2.frag", "Shader/geom2.geom");
	Model model_nanosuit("Model/nanosuit/nanosuit.obj");

	Shader shader_normal("Shader/vert3.vert", "Shader/frag3.frag", "Shader/geom3.geom");

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

		// render model
		shader_model.use();

		shader_model.setFloat("time", currentFrame);
		mat4 model = mat4(1.0F);
		model = translate(model, vec3(0.0f, -1.75f, 0.0f));
		model = scale(model, vec3(0.2f, 0.2f, 0.2f));
		shader_model.setMat4("model", model);
		mat4 view = camera.GetViewMatrix();
		shader_model.setMat4("view", view);
		mat4 projection = perspective(radians(camera.Zoom), 800.0F / 600.0F, 0.1F, 100.0F);
		shader_model.setMat4("projection", projection);
		model_nanosuit.Draw(shader_model);

		shader_normal.use();
		shader_normal.setMat4("model", model);
		shader_normal.setMat4("view", view);
		shader_normal.setMat4("projection", projection);
		model_nanosuit.Draw(shader_normal);


		//check and call event,swap buffer
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}


/////////////////////////////////////////////
// 下面是渲染简单的房子
/////////////////////////////////////////////
//void Draw() {
//	float points[] = {
//		-0.5f,  0.5f, 1.0f, 0.0f, 0.0f, // 左上
//		 0.5f,  0.5f, 0.0f, 1.0f, 0.0f, // 右上
//		 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // 右下
//		-0.5f, -0.5f, 1.0f, 1.0f, 0.0f  // 左下
//	};
//
//	glGenVertexArrays(1, &vao1);
//	glBindVertexArray(vao1);
//	glGenBuffers(1, &vbo1);
//	glBindBuffer(GL_ARRAY_BUFFER, vbo1);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
//	glEnableVertexAttribArray(0);
//	glVertexAttribPointer(0, 2, GL_FLOAT, false, 5 * sizeof(float), (void*)0);
//	glEnableVertexAttribArray(1);
//	glVertexAttribPointer(1, 3, GL_FLOAT, false, 5 * sizeof(float), (void*)(2 * sizeof(float)));
//	glBindBuffer(GL_ARRAY_BUFFER, 0);
//	glBindVertexArray(0);
//}
//void Render() {
//	Shader shader1 = Shader("Shader/vert1.vert", "Shader/frag1.frag","Shader/geom1.geom");
//
//	while (!glfwWindowShouldClose(window)) {
//		float currentFrame = glfwGetTime();
//		deltaTime = currentFrame - lastFrame;
//		lastFrame = currentFrame;
//
//		processInput(window);
//		glClearColor(0.0F, 0.0F, 0.0F, 1.0F);
//		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//		glBindVertexArray(vao1);
//		shader1.use();
//		glDrawArrays(GL_POINTS, 0, 4);
//
//		glBindVertexArray(0);
//
//		glfwSwapBuffers(window);
//		glfwPollEvents();
//	}
//}

