#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <iostream>
#include <Shader.h>
using namespace std;

void frameBuffer_size_callBack(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void Draw();
void UseShader();

const char *vertextShaderSource = "#version 330 core\n\
layout(location = 0) in vec3 aPos;\n\
layout(location = 1) in vec3 aColor;\n\
out vec3 vertexColor;\n\
void main() {\n\
	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n\
	vertexColor = aColor;\n\
}";

const char *fragmentShaderSource = "#version 330 core\n\
out vec4 FragColor;\n\
in vec3 vertexColor;\n\
void main() {\n\
	FragColor = vec4(vertexColor,1.0F);\n\
}";

unsigned int VAO;
unsigned int VBO;
unsigned int EBO;

Shader *theShader;

unsigned int shaderProgram;

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow *window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL) {
		cout << "Failed to create GLFW window" << endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		cout << "Failed to initialize GLAD" << endl;
		return -1;
	}

	glViewport(0, 0, 800, 600);

	glfwSetFramebufferSizeCallback(window, frameBuffer_size_callBack);

	//UseShader();

	theShader = new Shader("test1.vsh", "test1.fsh");
	theShader->setFloat("colorX", 1.0F);

	Draw();

	while (!glfwWindowShouldClose(window)) {

		//input
		processInput(window);

		//render
		glClearColor(0.2F, 0.3F, 0.3F, 1.0F);
		glClear(GL_COLOR_BUFFER_BIT);

		//draw
		//glUseProgram(shaderProgram);
		theShader->use();

		//float timeValue = glfwGetTime();
		//float greenValue = sin(timeValue) / 2.0F + 0.5F;
		//int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
		//glUniform4f(vertexColorLocation, 0.0F, greenValue, 0.0F, 1.0F);


		glBindVertexArray(VAO);

		glDrawArrays(GL_TRIANGLES, 0, 3);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);

		//check and call event,swap buffer
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//glDeleteVertexArrays(1, &VAO_TRIANGLE);
	//glDeleteBuffers(1, &VBO_TRIANGLE);

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	//glDeleteBuffers(1, &EBO);

	glfwTerminate();
	return 0;
}

void frameBuffer_size_callBack(GLFWwindow* window, int width, int height) {
	cout << "Test" << endl;
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

void Draw() {
	float vertices[] = {
		//pos color
		0.5F,-0.5F,0.0F,1.0F,0.0F,0.0F,
		-0.5F,-0.5F,0.0F,0.0F,1.0F,0.0F,
		0.0F,0.5F,0.0F,0.0F,0.0F,1.0F
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void UseShader() {
	//vertext
	unsigned int vertextShader;
	vertextShader = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vertextShader, 1, &vertextShaderSource, NULL);
	glCompileShader(vertextShader);

	//check error
	int success;
	char infoLog[512];
	glGetShaderiv(vertextShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertextShader, 512, NULL, infoLog);
		cout << "Error::vShader compile failed: " << infoLog << endl;
	}

	//fragment
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	//check error
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		cout << "Error::fShader compile failed: " << infoLog << endl;
	}

	//program
	shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertextShader);
	glAttachShader(shaderProgram, fragmentShader);

	glLinkProgram(shaderProgram);

	//check
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		cout << "Error::shaderProgram compile failed: " << infoLog << endl;
	}


	glDeleteShader(vertextShader);
	glDeleteShader(fragmentShader);

}
