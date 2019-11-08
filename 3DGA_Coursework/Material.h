#ifndef MATERIAL_H
#define MATERIAL_H

#include <glm/glm.hpp>

class Material
{
public:
	Material(glm::vec3& a, glm::vec3& d, glm::vec3 sp, float sh);

	static Material loadFromFile(const char* mtlPath);
private:
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shininess;
};
#endif // !MATERIAL_H
