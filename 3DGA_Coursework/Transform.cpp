#include <GLM/gtx/transform.hpp>

#include "Transform.h"
#include <glm/glm.hpp>

const glm::vec3 Transform::LEFT = glm::vec3(1.0f, 0.0f, 0.0f);
const glm::vec3 Transform::UP = glm::vec3(0.0f, 1.0f, 0.0f);
const glm::vec3 Transform::FORWARD = glm::vec3(0.0f, 0.0f, 1.0f);

Transform::Transform() 
{
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	scale = glm::vec3(1.0f, 1.0f, 1.0f);
	modelMat = glm::translate(glm::mat4(1.0f), position);
	modelMat = glm::rotate(modelMat, 0.0f, glm::vec3(Transform::LEFT));
	modelMat = glm::rotate(modelMat, 0.0f, glm::vec3(Transform::UP));
	modelMat = glm::rotate(modelMat, 0.0f, glm::vec3(Transform::FORWARD));
	modelMat = glm::scale(modelMat, scale);
	UpdateLocalVectors();
}

Transform::~Transform()
{
}

void Transform::Translate(glm::vec3 direction)
{
	position += direction;
	modelMat = glm::translate(glm::mat4(1.0f), position);
}

void Transform::Rotate(glm::vec3 axis, float angle_deg)
{
	if (axis.x > 0)
	{
		rotation.x += angle_deg;
	}
	else if (axis.y > 0)
	{
		rotation.y += angle_deg;
	}
	else if (axis.x > 0)
	{
		rotation.z += angle_deg;
	}
	
	modelMat = glm::rotate(modelMat, angle_deg, axis);
	UpdateLocalVectors();
}

void Transform::Scale(glm::vec3 scale)
{
	modelMat = glm::scale(modelMat, scale);
}

glm::vec3 Transform::GetPosition()
{
	return position;
}

glm::vec3 Transform::GetRotation()
{
	return rotation;
}

glm::vec3 Transform::GetScale()
{
	return scale;
}

glm::vec3 Transform::GetUp()
{
	return up;
}

glm::vec3 Transform::GetFront()
{
	return front;
}

glm::vec3 Transform::GetRight()
{
	return right;
}

void Transform::SetPosition(glm::vec3 pos)
{
	this->position = pos;
}

void Transform::SetRotation(glm::vec3 rot)
{
	this->rotation = rot;
	UpdateLocalVectors();
}

void Transform::SetScale(glm::vec3 scale)
{
	this->scale = scale;
}

void Transform::UpdateLocalVectors()
{
	glm::vec3 f;
	f.x = cos(glm::radians(rotation.y)) * cos(glm::radians(rotation.x));
	f.y = sin(glm::radians(rotation.x));
	f.z = sin(glm::radians(rotation.y)) * cos(glm::radians(rotation.x));

	front = glm::normalize(f);
	right = glm::normalize(glm::cross(front, Transform::UP));
	up = glm::normalize(glm::cross(right, front));
}
