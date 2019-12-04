#include "Material.h"

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
	this->shader->setUniform("light.position", glm::vec3(0.0f, 2.0f, .5f)); //TODO remove light
	this->shader->setUniform("light.ambient", glm::vec3(.2f, .2f, .2f));
	this->shader->setUniform("light.diffuse", glm::vec3(.5f, .5f, .5f));
	this->shader->setUniform("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));
	this->textureDiffuse->bind(0);
	this->textureSpecular->bind(1);
}

void Material::unuse()
{
	this->shader->unbind();
}
Shader* Material::GetShader()
{
	return this->shader;
}
