#ifndef POINT_LIGHT_H
#define POINT_LIGHT

#include "Light.h"

class PointLight : public Light
{
public:
	PointLight(glm::vec3 position, float a, float d, float s, float c, float l, float q);
	float constant;
	float linear;
	float quadratic;

};

PointLight::PointLight(glm::vec3 position, float a, float d, float s, float c, float l, float q) : Light(position,glm::vec3(),a,d,s)
{
	this->constant = c;
	this->linear = l;
	this->quadratic = q;
}

#endif // !POINT_LIGHT_H
