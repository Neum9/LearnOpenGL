#ifndef MESH_H
#define MESH_H

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

using namespace glm;
using namespace std;

struct Vertex {
	vec3 Position;
	vec3 Normal;
	vec2 TexCoords;
};

struct Texture {
	unsigned int id;
	string type;
	string path;
};

class Mesh {
public:
	//��������
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	vector<Texture> textures;
	//����
	Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures);
	void Draw(Shader shader);
private:
	//��Ⱦ����
	unsigned int VAO, VBO, EBO;
	//����
	void setupMesh();
};


#endif // !MESH_H