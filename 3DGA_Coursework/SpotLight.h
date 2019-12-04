#ifndef SPOT_LIGHT_H
#define SPOT_LIGHT_H

#include "PointLight.h"

class SpotLight : public PointLight
{
public:
	SpotLight(glm::vec3 position,glm::vec3 direction, float a, float d, float s, float c, float l, float q, float co, float oco);
	float cutoff;
	float outCutoff;
};

SpotLight::SpotLight(glm::vec3 position, glm::vec3 direction, float a, float d, float s, float c, float l, float q, float co, float oco) : PointLight(position,a,d,s, c,l,q)
{
	this->direction = direction;
	this->cutoff = co;
	this->outCutoff = oco;
}


#endif // !SPOT_LIGHT_H
