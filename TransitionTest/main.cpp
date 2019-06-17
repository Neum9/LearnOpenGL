#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;
using namespace std;


int main() {

	vec4 vec(1.0F, 0.0F, 0.0F, 1.0F);

	mat4 trans = mat4(1.0F);

	vec = trans * vec;

	trans = translate(trans, vec3(1.0F, 1.0F, 0.0F));

	vec = trans * vec;

	cout << vec.x << vec.y << vec.z << endl;

	system("pause");
	return 0;
}