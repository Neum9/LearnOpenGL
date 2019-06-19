#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;
using namespace std;


const unsigned int SCREEN_WIDTH = 800;
const unsigned int SCREEN_HEIGHT = 600;

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

	mat4 model = mat4(1.0F);
	model = rotate(model, radians(-55.0F), vec3(1.0F, 0.0F, 0.0F));

	mat4 view = mat4(1.0F);
	view = translate(view, vec3(0.0F, 0.0F, -3.0F));

	mat4 projection = mat4(1.0F);
	projection = perspective(radians(45.0F), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1F, 100.0F);



	return 0;
}