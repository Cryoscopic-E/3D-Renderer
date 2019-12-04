#ifndef MATERIAL_H
#define MATERIAL_H

#include <glm/glm.hpp>
#include <vector>
#include "Shader.h"
#include "Texture.h"
#include "Light.h"

using namespace Lights;

class Material
{
public:
	Material(float sh, Texture * textureDiffuse, Texture* textureSpecular, Shader* shader );
	void use();
	void unuse();

	void SetDirectionalLight(DirectionalLight* dl);
	void SetSpotLights(std::vector<SpotLight*> sl);
	void SetPointLights(std::vector<PointLight*> pl);

	Shader* GetShader();

	Texture* textureDiffuse;
	Texture* textureSpecular;
private:
	float shininess;
	DirectionalLight* directionalLight;
	std::vector<SpotLight*> spotLights;
	std::vector<PointLight*> pointLights;
	

	Shader* shader;
};
#endif // !MATERIAL_H
