#ifndef SHADER_H
#define SHADER_H

#include <glad\glad.h>
#include <GLFW\glfw3.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

class Shader {
public:
	//Program ID
	unsigned int ID;

	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);

#pragma region program
	void use();
#pragma endregion
	
#pragma region uniform Util
	void setBool(const string &name, bool value) const;
	void setInt(const string &name, int value)const;
	void setFloat(const string & name, float value) const;
#pragma endregion

};



#endif // !SHADER_H
