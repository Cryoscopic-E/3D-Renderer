#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <glm/glm.hpp>
#include <vector>

class Transform 
{
public:
	
	static const glm::vec3 UP;
	static const glm::vec3 FORWARD;
	static const glm::vec3 LEFT;

	Transform();

	void AddChild(Transform *child);

	void Translate(glm::vec3 direction);
	void Rotate(glm::vec3 axis, float angle_deg);
	void Scale(glm::vec3 scale);

	glm::vec3 GetPosition();
	glm::vec3 GetRotation();
	glm::vec3 GetScale();

	glm::vec3 GetUp();
	glm::vec3 GetFront();
	glm::vec3 GetRight();

	void SetPosition(glm::vec3 pos);
	void SetRotation(glm::vec3 rot);
	void SetScale(glm::vec3 scale);

	void CalculateModelMatrix();
	void UpdateLocalVectors();

	glm::mat4 GetModelMatrix();

private:

	Transform* parent;
	std::vector<Transform*> children;

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

	glm::vec3 up;
	glm::vec3 front;
	glm::vec3 right;

	glm::mat4 modelMat;

	
};
#endif // !TRANSFORM_H
