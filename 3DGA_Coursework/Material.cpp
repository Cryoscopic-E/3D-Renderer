#include "Material.h"
#include <string>

Material::Material(float sh, Texture* textureDiffuse, Texture* textureSpecular, Shader* shader)
{
	this->shininess = sh;

	this->textureDiffuse = textureDiffuse;
	this->textureSpecular = textureSpecular;

	this->shader = shader;
}

void Material::use()
{
	this->shader->bind();
	this->shader->setUniform("material.shininess", this->shininess);
	this->shader->setUniform("material.textureDiffuse", 0);
	this->shader->setUniform("material.textureSpecular", 1);
	//set uniforms:
	/*
	uniform DirectionalLight directionalLight;
	uniform PointLight pointLights[N_POINT_LIGHTS];
	uniform SpotLight spotLights[N_SPOT_LIGHTS];
	*/
	// directional Light
	this->shader->setUniform("directionalLight.direction", this->directionalLight->direction);
	this->shader->setUniform("directionalLight.ambient", this->directionalLight->ambient);
	this->shader->setUniform("directionalLight.diffuse", this->directionalLight->diffuse);
	this->shader->setUniform("directionalLight.specular", this->directionalLight->specular);

	//point lights
	for (int i = 0; i < this->pointLights.size(); i++)
	{
		std::string indx = std::to_string(i);
		this->shader->setUniform("pointLights[" + indx + "].position", this->pointLights[i]->position);
		this->shader->setUniform("pointLights[" + indx + "].ambient", this->pointLights[i]->ambient);
		this->shader->setUniform("pointLights[" + indx + "].diffuse", this->pointLights[i]->diffuse);
		this->shader->setUniform("pointLights[" + indx + "].specular", this->pointLights[i]->specular);
		this->shader->setUniform("pointLights[" + indx + "].constant", this->pointLights[i]->constant);
		this->shader->setUniform("pointLights[" + indx + "].linear", this->pointLights[i]->linear);
		this->shader->setUniform("pointLights[" + indx + "].quadratic", this->pointLights[i]->quadratic);
	}
	//spot lights
	for (int i = 0; i < this->spotLights.size(); i++)
	{
		std::string indx = std::to_string(i);
		this->shader->setUniform("spotLights[" + indx + "].position", this->spotLights[i]->position);
		this->shader->setUniform("spotLights[" + indx + "].ambient", this->spotLights[i]->ambient);
		this->shader->setUniform("spotLights[" + indx + "].diffuse", this->spotLights[i]->diffuse);
		this->shader->setUniform("spotLights[" + indx + "].specular", this->spotLights[i]->specular);
		this->shader->setUniform("spotLights[" + indx + "].constant", this->spotLights[i]->constant);
		this->shader->setUniform("spotLights[" + indx + "].linear", this->spotLights[i]->linear);
		this->shader->setUniform("spotLights[" + indx + "].quadratic", this->spotLights[i]->quadratic);
		this->shader->setUniform("spotLights[" + indx + "].cutOff", this->spotLights[i]->cutoff);
		this->shader->setUniform("spotLights[" + indx + "].outCutOff", this->spotLights[i]->outCutoff);
	}

	this->textureDiffuse->bind(0);
	this->textureSpecular->bind(1);
}

void Material::unuse()
{
	this->shader->unbind();
}
void Material::SetDirectionalLight(DirectionalLight* dl)
{
	this->directionalLight = dl;
}
void Material::SetSpotLights(std::vector<SpotLight*> sl)
{
	this->spotLights = sl;
}
void Material::SetPointLights(std::vector<PointLight*> pl)
{
	this->pointLights = pl;
}
Shader* Material::GetShader()
{
	return this->shader;
}
