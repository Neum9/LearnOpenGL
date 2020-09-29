#ifndef MESH_H
#define MESH_H

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Shader.hpp>
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
	//网格数据
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	vector<Texture> textures;
	//函数
	Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures);
	void Draw(Shader shader);
private:
	//渲染数据
	unsigned int VAO, VBO, EBO;
	//函数
	void setupMesh();
};


#endif // !MESH_H