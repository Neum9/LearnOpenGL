#define STB_IMAGE_IMPLEMENTATION
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Shader.hpp>
#include <stb_image.h>
#include <Camera.h>
#include <Model.h>

using namespace glm;
using namespace std;


const unsigned int SCREEN_WIDTH = 800;
const unsigned int SCREEN_HEIGHT = 600;

void GLFWInit();
void frameBuffer_size_callBack(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void Draw();
void Renderer();
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xOffset, double yOffset);
unsigned int LoadTexture(const char* name);

GLFWwindow *window;

unsigned int vaoObject;
unsigned int vboObject;

unsigned int vaoLight;
unsigned int vboLight;

mat4 model = mat4(1.0F);
mat4 view = mat4(1.0F);
mat4 projection = mat4(1.0F);


float deltaTime = 0.0F;
float lastFrame = 0.0F;

float lastX = 400, lastY = 300;

vec3 lightPos(0.0F, 0.0F, 1.0F);


Camera *camera = nullptr;

bool isFirstMouse = true;

glm::vec3 cubePositions[] = {
	glm::vec3(0.0f,  0.0f,  0.0f),
	glm::vec3(2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f,  2.0f, -2.5f),
	glm::vec3(1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
};

glm::vec3 pointLightPositions[] = {
	glm::vec3(0.7f,  0.2f,  2.0f),
	glm::vec3(2.3f, -3.3f, -4.0f),
	glm::vec3(-4.0f,  2.0f, -12.0f),
	glm::vec3(0.0f,  0.0f, -3.0f)
};

//获取数组长度
template <typename T, size_t N>
inline size_t Count(T(&arr)[N]) {
	return N;
}

int main() {


	GLFWInit();

	Draw();

	camera = new Camera(vec3(0.0F, 0.0F, 3.0F));

	Renderer();

	//glDeleteVertexArrays(1, &vaoObject);
	//glDeleteBuffers(1, &vboObject);

	glfwTerminate();

	return 0;
}

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


void processInput(GLFWwindow *window) {

	float cameraSpeed = 2.5F * deltaTime;

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {

		camera->ProcessKeyboard(FORWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {

		camera->ProcessKeyboard(BACKWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {

		camera->ProcessKeyboard(LEFT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {

		camera->ProcessKeyboard(RIGHT, deltaTime);
	}

}

//****************************************callback********************************************//

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

	camera->ProcessMouseMovement(xOffset, yOffset);
}

void scroll_callback(GLFWwindow* window, double xOffset, double yOffset) {

	camera->ProcessMouseScroll(yOffset);
}

//****************************************callback********************************************//

//传入点数据
void Draw() {


	float vertices[] = {
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
	};

	glGenVertexArrays(1, &vaoObject);
	glGenBuffers(1, &vboObject);

	glBindVertexArray(vaoObject);

	glBindBuffer(GL_ARRAY_BUFFER, vboObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(0));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glGenVertexArrays(1, &vaoLight);
	glGenBuffers(1, &vboLight);

	glBindVertexArray(vaoLight);
	glBindBuffer(GL_ARRAY_BUFFER, vboLight);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(0));
	glEnableVertexAttribArray(0);
}

//渲染
void Renderer() {


	glEnable(GL_DEPTH_TEST);

	Model ourModel("Model/nanosuit/nanosuit");
	Shader modelShader("Shader/model.vsh", "Shader/model.fsh");

	while (!glfwWindowShouldClose(window)) {

		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		//input
		processInput(window);

		//render
		glClearColor(0.1F, 0.1F, 0.1F, 1.0F);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		int modelLoc, viewLoc, projectionLoc;

		modelShader.use();

		mat4 model = mat4(1.0F);
		model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		modelShader.setMat4("model", model);
		view = camera->GetViewMatrix();
		modelShader.setMat4("view", view);
		projection = perspective(radians(camera->Zoom), 800.0F / 600.0F, 0.1F, 100.0F);
		modelShader.setMat4("projection", projection);
		ourModel.Draw(modelShader);

		//**************************************lightShader***************************************
		//lightShader.use();
		//view = camera->GetViewMatrix();
		//lightShader.setMat4("view", view);
		//projection = perspective(radians(camera->Zoom), 800.0F / 600.0F, 0.1F, 100.0F);
		//lightShader.setMat4("projection", projection);

		//glBindVertexArray(vaoLight);
		//for (int i = 0; i < Count(pointLightPositions); i++) {

		//	model = mat4(1.0F);
		//	model = translate(model, pointLightPositions[i]);
		//	model = scale(model, vec3(0.2F));
		//	lightShader.setMat4("model", model);

		//	glDrawArrays(GL_TRIANGLES, 0, 36);
		//}
		//**************************************lightShader***************************************

		//**************************************objectShader***************************************

		//objectShader.use();

		//objectShader.setVec3("light.ambient", 0.2F, 0.2F, 0.2F);
		//objectShader.setVec3("light.diffuse", 0.5F, 0.5F, 0.5F);
		//objectShader.setVec3("light.specular", 1.0F, 1.0F, 1.0F);
		//objectShader.setVec3("light.position", lightPos);

		//objectShader.setVec3("material.specular", 0.5F, 0.5F, 0.5F);
		//objectShader.setFloat("material.shininess", 64.0F);

		//objectShader.setVec3("viewPos", camera->Position);
		//model = mat4(1.0F);
		//objectShader.setMat4("model", model);
		//objectShader.setMat4("view", view);
		//objectShader.setMat4("projection", projection);

		//glBindVertexArray(vaoObject);

		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, diffuseTextureID);
		//glActiveTexture(GL_TEXTURE1);
		//glBindTexture(GL_TEXTURE_2D, specularTextureID);

		//glDrawArrays(GL_TRIANGLES, 0, 36);


		//**************************************objectShader***************************************

		//**************************************cubes***************************************
		//mulLightShader.use();

		//mulLightShader.setFloat("material.shininess", 64.0F);

		////定向光贡献
		//mulLightShader.setVec3("dirLight.ambient", 0.05F, 0.05F, 0.05F);
		//mulLightShader.setVec3("dirLight.diffuse", 0.4F, 0.4F, 0.4F);
		//mulLightShader.setVec3("dirLight.specular", 0.5F, 0.5F, 0.5F);
		//mulLightShader.setVec3("dirLight.direction", -0.2F, -1.0F, -0.3F);

		////点光源贡献
		//for (int i = 0; i < Count(pointLightPositions); i++) {
		//	char str[50];
		//	sprintf_s(str, 50, "pointLightList[%d].position", i);
		//	mulLightShader.setVec3(str, pointLightPositions[i]);
		//	//sprintf_s(str, "pointLightList[%d].cutOff", i);
		//	//mulLightShader.setFloat(str, cos(radians(12.5f)));
		//	//sprintf_s(str, "pointLightList[%d].outerCutOff", i);
		//	//mulLightShader.setFloat("pointLightList.outerCutOff", cos(radians(17.5f)));

		//	sprintf_s(str, 50, "pointLightList[%d].ambient", i);
		//	mulLightShader.setVec3(str, 0.05F, 0.05F, 0.05F);
		//	sprintf_s(str, 50, "pointLightList[%d].diffuse", i);
		//	mulLightShader.setVec3(str, 0.8F, 0.8F, 0.8F);
		//	sprintf_s(str, 50, "pointLightList[%d].specular", i);
		//	mulLightShader.setVec3(str, 1.0F, 1.0F, 1.0F);

		//	sprintf_s(str, 50, "pointLightList[%d].constant", i);
		//	mulLightShader.setFloat(str, 1.0F);
		//	sprintf_s(str, 50, "pointLightList[%d].linear", i);
		//	mulLightShader.setFloat(str, 0.09F);
		//	sprintf_s(str, 50, "pointLightList[%d].quadratic", i);
		//	mulLightShader.setFloat(str, 0.032F);
		//}

		////聚光灯贡献
		//mulLightShader.setVec3("spotLight.position", camera->Position);
		//mulLightShader.setVec3("spotLight.direction", camera->Front);
		//mulLightShader.setFloat("spotLight.cutOff", cos(radians(12.5f)));
		//mulLightShader.setFloat("spotLight.outerCutOff", cos(radians(15.0f)));

		//mulLightShader.setVec3("spotLight.ambient", 0.0F, 0.0F, 0.0F);
		//mulLightShader.setVec3("spotLight.diffuse", 1.0F, 1.0F, 1.0F);
		//mulLightShader.setVec3("spotLight.specular", 1.0F, 1.0F, 1.0F);

		//mulLightShader.setFloat("spotLight.constant", 1.0F);
		//mulLightShader.setFloat("spotLight.linear", 0.09F);
		//mulLightShader.setFloat("spotLight.quadratic", 0.032F);

		////视点
		//mulLightShader.setVec3("viewPos", camera->Position);
		////矩阵
		//view = camera->GetViewMatrix();
		//mulLightShader.setMat4("view", view);
		//projection = perspective(radians(camera->Zoom), 800.0F / 600.0F, 0.1F, 100.0F);
		//mulLightShader.setMat4("projection", projection);

		//glBindVertexArray(vaoObject);

		////不同的位置矩阵
		//for (unsigned int i = 0; i < 10; i++) {
		//	model = mat4(1.0F);
		//	model = translate(model, cubePositions[i]);
		//	float angle = 20.0f * i;
		//	model = rotate(model, radians(angle), vec3(1.0f, 0.3f, 0.5f));
		//	mulLightShader.setMat4("model", model);

		//	glActiveTexture(GL_TEXTURE0);
		//	glBindTexture(GL_TEXTURE_2D, diffuseTextureID);
		//	glActiveTexture(GL_TEXTURE1);
		//	glBindTexture(GL_TEXTURE_2D, specularTextureID);

		//	glDrawArrays(GL_TRIANGLES, 0, 36);
		//}

		//**************************************cubes***************************************

		//check and call event,swap buffer
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

//加载图片
unsigned int LoadTexture(const char* name) {

	stbi_set_flip_vertically_on_load(true);

	int width, height, nrChannels;
	unsigned char *data = stbi_load(name, &width, &height, &nrChannels, 0);

	unsigned int textureID;

	glGenTextures(1, &textureID);

	//glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);

	if (data) {

		GLenum format;
		if (nrChannels == 1)
			format = GL_RED;
		else if (nrChannels == 3)
			format = GL_RGB;
		else if (nrChannels == 4)
			format = GL_RGBA;

		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	} else {
		cout << "Failed to load texture" << endl;
	}

	stbi_image_free(data);

	return textureID;
}