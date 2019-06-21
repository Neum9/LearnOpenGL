#define STB_IMAGE_IMPLEMENTATION
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Shader.h>
#include <stb_image.h>

using namespace glm;
using namespace std;


const unsigned int SCREEN_WIDTH = 800;
const unsigned int SCREEN_HEIGHT = 600;

void GLFWInit();
void frameBuffer_size_callBack(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void Draw();
void Renderer();
void InitMatrix();
void DrawCube();

GLFWwindow *window;

unsigned int VAO;
unsigned int VBO;
unsigned int EBO;


mat4 model = mat4(1.0F);
mat4 view = mat4(1.0F);
mat4 projection = mat4(1.0F);

#pragma region cube

unsigned int VAO_CUBE;
unsigned int VBO_CUBE;
unsigned int EBO_CUBE;

#pragma endregion



int main() {
#pragma region DataTest


	//vec4 vec(1.0F, 0.0F, 0.0F, 1.0F);

	//mat4 trans = mat4(1.0F);

	////获得位移矩阵
	//trans = translate(trans, vec3(1.0F, 1.0F, 0.0F));

	//vec = trans * vec;

	////结果为(2,1,0)
	//cout << vec.x << vec.y << vec.z << endl;

	//system("pause");
#pragma endregion

	GLFWInit();

	Draw();

	InitMatrix();

	Renderer();

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
	}
}

//绘制图片
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
	unsigned char *data = stbi_load("Texture/awesomeface.jpg", &width, &height, &nrChannels, 0);

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

	data = stbi_load("Texture/brick.jpg", &width, &height, &nrChannels, 0);
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
}

//绘制立方体
void DrawCube() {

	//flip
	stbi_set_flip_vertically_on_load(true);

	//maybe texture would be okasi with the okasi textureCoord
	float vertices[] = {
		//Pos               TextureCoord
		0.5F,0.5F,0.5F,      1.0F,1.0F,
		0.5F,-0.5F,0.5F,	1.0F,0.0F,
		-0.5F,-0.5F,0.5F,	0.0F,0.0F,
		-0.5F,0.5F,0.5F,	0.0F,1.0F,
		0.5F,0.5F,-0.5F,	1.0F,1.0F,
		0.5F,-0.5F,-0.5F,	1.0F,0.0F,
		-0.5F,-0.5F,-0.5F,	0.0F,0.0F,
		-0.5F,0.5F,-0.5F,	0.0F,1.0F,
	};

	unsigned int indexes[] = {
		0,1,2,
		2,3,0,
		4,5,1,
		1,0,4,
		4,0,3,
		3,7,4,
		3,2,6,
		6,7,3,
		1,5,6,
		6,2,1,
		7,6,5,
		5,4,7
	};

	glGenVertexArrays(1, &VAO_CUBE);
	glGenBuffers(1, &VBO_CUBE);
	glGenBuffers(1, &EBO_CUBE);

	glBindVertexArray(VAO_CUBE);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_CUBE);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_CUBE);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexes), indexes, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)3);
	glEnableVertexAttribArray(1);

}

//渲染
void Renderer() {

	Shader shader("Shader/test.vsh", "Shader/test.fsh");

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

		//model
		int modelLoc = glGetUniformLocation(shader.ID, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(model));
		int viewLoc = glGetUniformLocation(shader.ID, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, value_ptr(view));
		int projectionLoc = glGetUniformLocation(shader.ID, "projection");
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, value_ptr(projection));


		glBindVertexArray(VAO);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);

		//check and call event,swap buffer
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

//初始化图片
void InitMatrix() {
	model = rotate(model, radians(-55.0F), vec3(1.0F, 0.0F, 0.0F));

	view = translate(view, vec3(0.0F, 0.0F, -3.0F));

	projection = perspective(radians(45.0F), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1F, 100.0F);
}