#ifndef MATERIAL_H
#define MATERIAL_H

#include <glm/glm.hpp>
#include "Shader.h"
#include "Texture.h"
class Material
{
public:
	Material(glm::vec3 a, glm::vec3 d, glm::vec3 sp, float sh, Texture * textureDiffuse, Texture* textureSpecular, Shader* shader);
	void use();
	void unuse();

	Shader* GetShader();

	Texture* textureDiffuse;
	Texture* textureSpecular;
private:
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shininess;

	

	Shader* shader;
};
#endif // !MATERIAL_H
