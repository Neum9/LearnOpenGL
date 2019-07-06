#ifndef MODEL_H
#define MODEL_H

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Shader.hpp>
#include <Camera.h>
#include <Mesh.h>
#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

using namespace glm;
using namespace std;
using namespace Assimp;

unsigned int TextureFromFile(const char *path, const string &directory, bool gamma = false);

class Model {
public:
	//����
	Model(char *path) {
		loadModel(path);
	}

	void Draw(Shader shader);
private:
	vector<Texture> textures_loaded;
	//ģ������
	vector<Mesh> meshes;
	string directory;
	//����
	void loadModel(string path);
	void processNode(aiNode * node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName);
};


#endif // !MODEL_H