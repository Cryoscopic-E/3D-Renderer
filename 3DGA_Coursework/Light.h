#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>

namespace Lights
{
	

	struct PointLight
	{
	public:
		PointLight(glm::vec3 pos, glm::vec3 a, glm::vec3 d, glm::vec3 s, float c, float l, float q)
		{
			position = pos;
			ambient = a;
			specular = s;
			diffuse = d;
			constant = c;
			linear = l;
			quadratic = q;
		};

		glm::vec3 position;
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;
		float constant;
		float linear;
		float quadratic;
	};


	class SpotLight
	{
	public:
		SpotLight(glm::vec3 pos, glm::vec3 dir, glm::vec3 a, glm::vec3 d, glm::vec3 s, float c, float l, float q, float co, float oco)
		{
			position = pos;
			direction = dir;
			ambient = a;
			specular = s;
			diffuse = d;
			constant = c;
			linear = l;
			quadratic = q;
			cutoff = co;
			outCutoff = oco;
		};
		glm::vec3 position;
		glm::vec3 direction;
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;
		float constant;
		float linear;
		float quadratic;
		float cutoff;
		float outCutoff;
	};
	struct DirectionalLight
	{
	public:
		DirectionalLight(glm::vec3 dir, glm::vec3 a, glm::vec3 d, glm::vec3 s)
		{
			direction = dir;
			ambient = a;
			specular = s;
			diffuse = d;
		}
		glm::vec3 direction;
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;
	};
}


#endif // !LIGHT_H
