///*
//	帧缓冲
//	https://learnopengl-cn.github.io/04%20Advanced%20OpenGL/05%20Framebuffers/
//*/
//
//#define STB_IMAGE_IMPLEMENTATION
//#include <iostream>
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>
//#include <glad/glad.h>
//#include <GLFW/glfw3.h>
//#include <Shader.hpp>
//#include <Camera.h>
//#include <stb_image.h>
//
//using namespace std;
//using namespace glm;
//
//const unsigned int SCREEN_WIDTH = 800;
//const unsigned int SCREEN_HEIGHT = 600;
//
//void GLFWInit();
//void frameBuffer_size_callBack(GLFWwindow* window, int width, int height);
//void processInput(GLFWwindow *window);
//void Draw();
//void Renderer();
//void Renderer1();
//void mouse_callback(GLFWwindow* window, double xpos, double ypos);
//void scroll_callback(GLFWwindow* window, double xOffset, double yOffset);
//unsigned int LoadTexture(const char* name);
//void TextureEnclosure();
//void DrawBufferObject();
//
//GLFWwindow *window;
//
//unsigned int VAO;
//unsigned int VBO;
//unsigned int EBO;
//
//unsigned int fbo;
//unsigned int rbo;
//
//unsigned int vaoQuad;
//unsigned int vboQuad;
//
//unsigned int lightVAO;
//
//vec3 lightPos(1.2F, 1.0F, 2.0F);
//
//Camera camera(vec3(0.0F, 0.0F, 3.0F));
//
//float lastX = 400, lastY = 300;
//bool isFirstMouse = true;
//
//float deltaTime = 0.0F;
//float lastFrame = 0.0F;
//
//float specularStrength = 0.5F;
//
////Shader screenShader("Shader/frameBuffer.vsh","Shader/frameBuffer.fsh");
//Shader* screenShader;
//
//unsigned int textureFrameBuffer;
//
//int main() {
//	GLFWInit();
//
//	Draw();
//
//	//Renderer();
//	Renderer1();
//
//	glfwTerminate();
//
//	glDeleteVertexArrays(1, &VAO);
//	glDeleteBuffers(1, &VBO);
//
//	return 0;
//}
//
//void GLFWInit() {
//	glfwInit();
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//
//	window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
//	if (window == NULL) {
//		cout << "Failed to create GLFW window" << endl;
//		glfwTerminate();
//		exit(-1);
//	}
//
//	glfwMakeContextCurrent(window);
//
//	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
//		cout << "Failed to initialize GLAD" << endl;
//		exit(-1);
//	}
//
//	glViewport(0, 0, 800, 600);
//
//	glfwSetFramebufferSizeCallback(window, frameBuffer_size_callBack);
//
//	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
//
//	glfwSetCursorPosCallback(window, mouse_callback);
//
//	glfwSetScrollCallback(window, scroll_callback);
//}
//
//void processInput(GLFWwindow *window) {
//
//	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
//		glfwSetWindowShouldClose(window, true);
//	}
//	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
//		//cameraPos += cameraSpeed * cameraFront;
//		camera.ProcessKeyboard(FORWARD, deltaTime);
//	}
//	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
//		//cameraPos -= cameraSpeed * cameraFront;
//		camera.ProcessKeyboard(BACKWARD, deltaTime);
//	}
//	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
//		//cameraPos -= normalize(cross(cameraFront, cameraUp)) * cameraSpeed;
//		camera.ProcessKeyboard(LEFT, deltaTime);
//	}
//	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
//		//cameraPos += normalize(cross(cameraFront, cameraUp)) * cameraSpeed;
//		camera.ProcessKeyboard(RIGHT, deltaTime);
//	}
//}
//
//void frameBuffer_size_callBack(GLFWwindow* window, int width, int height) {
//	glViewport(0, 0, width, height);
//}
//
//void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
//	if (isFirstMouse) {
//		lastX = xpos;
//		lastY = ypos;
//		isFirstMouse = false;
//	}
//
//	float xOffset = xpos - lastX;
//	float yOffset = lastY - ypos;
//
//	lastX = xpos;
//	lastY = ypos;
//	camera.ProcessMouseMovement(xOffset, yOffset);
//}
//
//void scroll_callback(GLFWwindow* window, double xOffset, double yOffset) {
//	camera.ProcessMouseScroll(yOffset);
//}
//
//void Draw() {
//	float vertices[] = {
//		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
//		0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
//		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
//		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
//		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
//		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
//
//		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
//		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
//		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
//		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
//		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
//		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
//
//		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
//		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
//		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
//		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
//		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
//		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
//
//		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
//		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
//		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
//		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
//		0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
//		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
//
//		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
//		0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
//		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
//		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
//		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
//		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
//
//		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
//		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
//		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
//		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
//		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
//		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
//	};
//
//	float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
//	// positions   // texCoords
//	-1.0f,  1.0f,  0.0f, 1.0f,
//	-1.0f, -1.0f,  0.0f, 0.0f,
//	 1.0f, -1.0f,  1.0f, 0.0f,
//
//	-1.0f,  1.0f,  0.0f, 1.0f,
//	 1.0f, -1.0f,  1.0f, 0.0f,
//	 1.0f,  1.0f,  1.0f, 1.0f
//	};
//	//////////////////////////////////////object
//	glGenVertexArrays(1, &VAO);
//	glGenBuffers(1, &VBO);
//
//	glBindVertexArray(VAO);
//
//	glBindBuffer(GL_ARRAY_BUFFER, VBO);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
//	
//	glEnableVertexAttribArray(0);
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(0));
//
//	glEnableVertexAttribArray(1);
//	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
//
//	glBindBuffer(GL_ARRAY_BUFFER, 0);
//	glBindVertexArray(0);
//
//	//////////////////////////////////////quad
//	glGenVertexArrays(1, &vaoQuad);
//	glGenBuffers(1, &vboQuad);
//
//	glBindVertexArray(vaoQuad);
//
//	glBindBuffer(GL_ARRAY_BUFFER, vboQuad);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
//
//	glEnableVertexAttribArray(0);
//	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
//
//	glEnableVertexAttribArray(1);
//	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
//
//	glBindBuffer(GL_ARRAY_BUFFER, 0);
//	glBindVertexArray(0);
//
//	//////////////////////////////////////light
//	//glGenVertexArrays(1, &lightVAO);
//	//glBindVertexArray(lightVAO);
//
//	//glBindBuffer(GL_ARRAY_BUFFER, VBO);
//	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(0));
//	//glEnableVertexAttribArray(0);
//
//	//glBindVertexArray(0);
//
//	//创建帧缓冲
//	unsigned int fbo;
//	glGenFramebuffers(1, &fbo);
//	//绑定帧缓冲
//	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
//
//	TextureEnclosure();
//	DrawBufferObject();
//
//
//	//检测帧缓冲是否完整
//	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
//		cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
//	}
//	//激活默认帧缓存 绑定到0
//	glBindFramebuffer(GL_FRAMEBUFFER, 0);
//	////删除帧缓冲对象
//	//glDeleteFramebuffers(1, &fbo);
//
//}
//
////纹理附件
//void TextureEnclosure() {
//	glGenTextures(1, &textureFrameBuffer);
//	glBindTexture(GL_TEXTURE_2D, textureFrameBuffer);
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	//附加到当前绑定的帧缓冲对象
//	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureFrameBuffer, 0);
//}
//
////渲染缓冲对象附件
//void DrawBufferObject() {
//	//创建渲染缓冲对象
//	glGenRenderbuffers(1, &rbo);
//	//绑定渲染缓冲对象
//	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
//	//创建深度和模板渲染缓冲对象
//	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600);
//	//附加渲染缓冲对象
//	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
//}
//
////渲染到纹理
//void RenderWithFrameBuffer() {
//	//返回默认
//	glBindFramebuffer(GL_FRAMEBUFFER, 0);
//	glDisable(GL_DEPTH_TEST);
//	glClearColor(1.0F, 1.0F, 1.0F, 1.0F);
//	glClear(GL_COLOR_BUFFER_BIT);
//
//
//	screenShader->use();
//	glBindVertexArray(vaoQuad);
//	glBindTexture(GL_TEXTURE_2D, textureFrameBuffer);
//
//	glDrawArrays(GL_TRIANGLES, 0, 6);
//}
//
//void Renderer() {
//	Shader objectShader("Shader/object.vsh", "Shader/object.fsh");
//
//	screenShader = new Shader("Shader/frameBuffer.vsh", "Shader/frameBuffer.fsh");
//
//	int textureID = LoadTexture("Texture/container2.png");
//
//	objectShader.use();
//	glActiveTexture(GL_TEXTURE0);
//	objectShader.setInt("texture1", 0);
//
//	//screenShader->use();
//	//glActiveTexture(GL_TEXTURE0);
//	//screenShader->setInt("screenTexture", 0);
//
//	glEnable(GL_DEPTH_TEST);
//	
//	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
//
//	while (!glfwWindowShouldClose(window)) {
//
//		float currentFrame = glfwGetTime();
//		deltaTime = currentFrame - lastFrame;
//		lastFrame = currentFrame;
//
//		//input
//		processInput(window);
//		//clear cache
//		//glClearColor(0.2F, 0.3F, 0.3F, 1.0F);
//		glClearColor(0.0F, 0.0F, 0.0F, 1.0F);
//
//		//**********************************object***********************************************
//		//object		
//		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//		objectShader.use();
//		glBindTexture(GL_TEXTURE_2D, textureID);
//		mat4 model = mat4(1.0F);
//		int modelLoc = glGetUniformLocation(objectShader.ID, "model");
//		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(model));
//		mat4 view = camera.GetViewMatrix();
//		int viewLoc = glGetUniformLocation(objectShader.ID, "view");
//		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, value_ptr(view));
//		mat4 projection = perspective(radians(camera.Zoom), 800.0F / 600.0F, 0.1F, 100.0F);
//		int projectionLoc = glGetUniformLocation(objectShader.ID, "projection");
//		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, value_ptr(projection));
//		//render
//		glBindVertexArray(VAO);
//
//		glDrawArrays(GL_TRIANGLES, 0, 36);
//		glBindVertexArray(0);
//
//		//////////////////////////////////////screen
//		RenderWithFrameBuffer();
//
//		//check and call event,swap buffer
//		glfwSwapBuffers(window);
//		glfwPollEvents();
//	}
//}
//
//void Renderer1() {
//	Shader objectShader("Shader/object.vsh", "Shader/object.fsh");
//
//	screenShader = new Shader("Shader/frameBuffer.vsh", "Shader/frameBuffer.fsh");
//
//	int textureID = LoadTexture("Texture/container2.png");
//
//	objectShader.use();
//	//glActiveTexture(GL_TEXTURE0);
//	objectShader.setInt("texture1", 0);
//
//	screenShader->use();
//	//glActiveTexture(GL_TEXTURE0);
//	screenShader->setInt("screenTexture", 0);
//
//	//glEnable(GL_DEPTH_TEST);
//
//	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
//
//	while (!glfwWindowShouldClose(window)) {
//
//		float currentFrame = glfwGetTime();
//		deltaTime = currentFrame - lastFrame;
//		lastFrame = currentFrame;
//
//		//input
//		processInput(window);
//
//		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
//		glEnable(GL_DEPTH_TEST); // enable depth testing (is disabled for rendering screen-space quad)
//
//		// make sure we clear the framebuffer's content
//		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
//		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//		objectShader.use();
//
//		glm::mat4 model = glm::mat4(1.0f);
//		glm::mat4 view = camera.GetViewMatrix();
//		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)800 / (float)600, 0.1f, 100.0f);
//		objectShader.setMat4("view", view);
//		objectShader.setMat4("projection", projection);
//		// cubes
//		glBindVertexArray(VAO);
//		glActiveTexture(GL_TEXTURE0);
//		glBindTexture(GL_TEXTURE_2D, textureID);
//		model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
//		objectShader.setMat4("model", model);
//		glDrawArrays(GL_TRIANGLES, 0, 36);
//		model = glm::mat4(1.0f);
//		model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
//		objectShader.setMat4("model", model);
//		glDrawArrays(GL_TRIANGLES, 0, 36);
//
//
//		//////////////////////////////////////screen
//		RenderWithFrameBuffer();
//
//		//check and call event,swap buffer
//		glfwSwapBuffers(window);
//		glfwPollEvents();
//	}
//}
//
////加载图片
//unsigned int LoadTexture(const char* name) {
//
//	stbi_set_flip_vertically_on_load(true);
//
//	int width, height, nrChannels;
//	unsigned char *data = stbi_load(name, &width, &height, &nrChannels, 0);
//
//	unsigned int textureID;
//
//	glGenTextures(1, &textureID);
//
//	//glActiveTexture(GL_TEXTURE0);
//	glBindTexture(GL_TEXTURE_2D, textureID);
//
//	if (data) {
//
//		GLenum format;
//		if (nrChannels == 1)
//			format = GL_RED;
//		else if (nrChannels == 3)
//			format = GL_RGB;
//		else if (nrChannels == 4)
//			format = GL_RGBA;
//
//		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
//		glGenerateMipmap(GL_TEXTURE_2D);
//
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//
//	} else {
//		cout << "Failed to load texture" << endl;
//	}
//
//	stbi_image_free(data);
//
//	return textureID;
//}

//#include <glad/glad.h>
//#include <GLFW/glfw3.h>
//#include <stb_image.h>
//
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>
//
//#include <learnopengl/shader_m.h>
//#include <learnopengl/camera.h>
//#include <learnopengl/model.h>
//#include <iostream>


#define STB_IMAGE_IMPLEMENTATION

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Shader.hpp>
#include <Camera.h>
#include <stb_image.h>

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
unsigned int loadTexture(const char* path);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = (float)SCR_WIDTH / 2.0;
float lastY = (float)SCR_HEIGHT / 2.0;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main()
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
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile shaders
    // -------------------------

    //	Shader objectShader("Shader/object.vsh", "Shader/object.fsh");
//
//	screenShader = new Shader("Shader/frameBuffer.vsh", "Shader/frameBuffer.fsh");

    Shader shader("Shader/object.vsh", "Shader/object.fsh");
    Shader screenShader("Shader/frameBuffer.vsh", "Shader/frameBuffer.fsh");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
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
    float planeVertices[] = {
        // positions          // texture Coords 
         5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
        -5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
        -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,

         5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
        -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
         5.0f, -0.5f, -5.0f,  2.0f, 2.0f
    };
    float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
        // positions   // texCoords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };
    // cube VAO
    unsigned int cubeVAO, cubeVBO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    // plane VAO
    unsigned int planeVAO, planeVBO;
    glGenVertexArrays(1, &planeVAO);
    glGenBuffers(1, &planeVBO);
    glBindVertexArray(planeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    // screen quad VAO
    unsigned int quadVAO, quadVBO;
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

    // load textures
    // -------------
    unsigned int cubeTexture = loadTexture("Texture/container2.png");
    unsigned int floorTexture = loadTexture("Texture/container2.png");

    // shader configuration
    // --------------------
    shader.use();
    shader.setInt("texture1", 0);

    screenShader.use();
    screenShader.setInt("screenTexture", 0);

    // framebuffer configuration
    // -------------------------
    unsigned int framebuffer;
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    // create a color attachment texture
    unsigned int textureColorbuffer;
    glGenTextures(1, &textureColorbuffer);
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
    // create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
    unsigned int rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT); // use a single renderbuffer object for both a depth AND stencil buffer.
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it
    // now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // draw as wireframe
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);


        // render
        // ------
        // bind to framebuffer and draw scene as we normally would to color texture 
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        glEnable(GL_DEPTH_TEST); // enable depth testing (is disabled for rendering screen-space quad)

        // make sure we clear the framebuffer's content
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);
        // cubes
        glBindVertexArray(cubeVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, cubeTexture);
        model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
        shader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
        shader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        // floor
        glBindVertexArray(planeVAO);
        glBindTexture(GL_TEXTURE_2D, floorTexture);
        shader.setMat4("model", glm::mat4(1.0f));
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);

        // now bind back to default framebuffer and draw a quad plane with the attached framebuffer color texture
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.
        // clear all relevant buffers
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // set clear color to white (not really necessery actually, since we won't be able to see behind the quad anyways)
        glClear(GL_COLOR_BUFFER_BIT);

        screenShader.use();
        glBindVertexArray(quadVAO);
        glBindTexture(GL_TEXTURE_2D, textureColorbuffer);	// use the color attachment texture as the texture of the quad plane
        glDrawArrays(GL_TRIANGLES, 0, 6);


        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &planeVAO);
    glDeleteVertexArrays(1, &quadVAO);
    glDeleteBuffers(1, &cubeVBO);
    glDeleteBuffers(1, &planeVBO);
    glDeleteBuffers(1, &quadVBO);

    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
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

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}

// utility function for loading a 2D texture from file
// ---------------------------------------------------
unsigned int loadTexture(char const* path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}