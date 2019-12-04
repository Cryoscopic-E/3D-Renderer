#ifndef DIRECTIONAL_LIGHT
#define DIRECTIONAL_LIGHT
#include "Light.h"


class DirectionalLight:Light
{
public:
	DirectionalLight(glm::vec3 direction, float a, float d, float s);

};

DirectionalLight::DirectionalLight(glm::vec3 direction, float a, float d, float s) : Light(glm::vec3(),direction,a,d,s)
{
}
#endif // !DIRECTIONAL_LIGHT
