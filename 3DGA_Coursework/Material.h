#ifndef MATERIAL_H
#define MATERIAL_H

#include <glm/glm.hpp>
#include "Shader.h"
#include "Texture.h"
class Material
{
public:
	Material(float sh, Texture * textureDiffuse, Texture* textureSpecular, Shader* shader);
	void use();
	void unuse();

	Shader* GetShader();

	Texture* textureDiffuse;
	Texture* textureSpecular;
private:
	float shininess;

	

	Shader* shader;
};
#endif // !MATERIAL_H
