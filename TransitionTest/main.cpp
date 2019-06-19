#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;
using namespace std;


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

	return 0;
}