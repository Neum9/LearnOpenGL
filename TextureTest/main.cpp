#define STB_IMAGE_IMPLEMENTATION
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <iostream>
#include <Shader.h>
#include <glm\glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;
using namespace glm;

void GLFWInit();
void frameBuffer_size_callBack(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void Draw();

GLFWwindow *window;

unsigned int VAO;
unsigned int VBO;
unsigned int EBO;

float m_coe = 0.5F;

mat4 trans = mat4(1.0F);

int main() {

	GLFWInit();

	Shader shader("Shader/test.vsh", "Shader/test1.fsh");

	Draw();

	unsigned int transfomrLoc = glGetUniformLocation(shader.ID, "transform");

	while (!glfwWindowShouldClose(window)) {

		//input
		processInput(window);

		//render
		glClearColor(0.2F, 0.3F, 0.3F, 1.0F);
		glClear(GL_COLOR_BUFFER_BIT);

		//draw
		shader.use();
		shader.setInt("texture1", 0);
		shader.setInt("texture2", 1);

		mat4 trans1 = mat4(1.0F);
		trans1 = rotate(trans1, (float)glfwGetTime(), vec3(0.0F, 0.0F, 1.0F));
		glUniformMatrix4fv(transfomrLoc, 1, GL_FALSE, value_ptr(trans));

		shader.setFloat("m_coe", m_coe);

		glBindVertexArray(VAO);

		//glDrawArrays(GL_TRIANGLES, 0, 3);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


		mat4 trans2 = mat4(1.0F);
		trans2 = translate(trans2, vec3(1.0F, 1.0F, 0.0F));
		glUniformMatrix4fv(transfomrLoc, 1, GL_FALSE, value_ptr(trans2));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);

		//check and call event,swap buffer
		glfwSwapBuffers(window);
		glfwPollEvents();
	}


	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glfwTerminate();


	return 0;
}

void GLFWInit() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

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
}

void frameBuffer_size_callBack(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	} else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		m_coe = m_coe + 0.1 > 1 ? 1 : m_coe + 0.1;
	} else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		m_coe = m_coe - 0.1 < 0 ? 0 : m_coe - 0.1;
	}
}

void Draw() {

	stbi_set_flip_vertically_on_load(true);

	float vertices[] = {
		//--Pos--			--Color--	--TextureCoord--
		0.5F,0.5F,0.0F,		1.0F,0.0F,0.0F,	1.0F,1.0F,	//top right
		0.5F,-0.5F,0.0F,	0.0F,1.0F,0.0F,	1.0F,0.0F,	//bottom right
		-0.5F,-0.5F,0.0F,	0.0F,0.0F,1.0F,	0.0F,0.0F,	//bottom left
		-0.5F,0.5F,0.0F,	1.0F,1.0F,0.0F,	0.0F,1.0F	//top left
	};

	unsigned int indexes[] = {
		0,1,2,
		2,3,0
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexes), indexes, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(0));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	int width, height, nrChannels;

	//unsigned char *data = stbi_load("brick.jpg", &width, &height, &nrChannels, 0);
	unsigned char *data = stbi_load("awesomeface.jpg", &width, &height, &nrChannels, 0);

	unsigned int texture;

	glGenTextures(1, &texture);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

		glGenerateMipmap(GL_TEXTURE_2D);
	} else {
		cout << "Failed to load texture" << endl;
	}

	stbi_image_free(data);

	data = stbi_load("brick.jpg", &width, &height, &nrChannels, 0);
	unsigned int texture2;
	glGenTextures(1, &texture2);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);

	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

		glGenerateMipmap(GL_TEXTURE_2D);
	} else {
		cout << "Failed to load texture" << endl;
	}

	stbi_image_free(data);


	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	trans = scale(trans, vec3(0.5, 0.5, 0.5));
	trans = rotate(trans, radians(90.0F), vec3(0.0F, 0.0F, 1.0F));
}
