#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>

class Light
{
public:
	Light(glm::vec3 position, glm::vec3 direction, float a, float d, float s);
	glm::vec3 position;
	glm::vec3 direction;
	float ambient;
	float diffuse;
	float specular;
};

Light::Light(glm::vec3 position, glm::vec3 direction, float a, float d, float s)
{
	this->position = position;
	this->direction = direction;
	this->ambient = a;
	this->specular = s;
	this->diffuse = d;
}

#endif // !LIGHT_H
