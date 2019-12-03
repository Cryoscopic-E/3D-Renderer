#include <GLM/gtx/transform.hpp>

#include "Transform.h"
#include <glm/glm.hpp>

const glm::vec3 Transform::LEFT = glm::vec3(1.0f, 0.0f, 0.0f);
const glm::vec3 Transform::UP = glm::vec3(0.0f, 1.0f, 0.0f);
const glm::vec3 Transform::FORWARD = glm::vec3(0.0f, 0.0f, 1.0f);

Transform::Transform() 
{
	this->position = glm::vec3(0.0f, 0.0f, 0.0f);
	this->rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	this->scale = glm::vec3(1.0f, 1.0f, 1.0f);
	this->modelMat = glm::translate(glm::mat4(1.0f), position);
	this->modelMat = glm::rotate(modelMat, 0.0f, glm::vec3(Transform::LEFT));
	this->modelMat = glm::rotate(modelMat, 0.0f, glm::vec3(Transform::UP));
	this->modelMat = glm::rotate(modelMat, 0.0f, glm::vec3(Transform::FORWARD));
	this->modelMat = glm::scale(modelMat, scale);
	this->UpdateLocalVectors();
}

void Transform::Translate(glm::vec3 direction)
{
	this->position += direction;
	this->modelMat = glm::translate(glm::mat4(1.0f), position);
	this->UpdateLocalVectors();
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
	
	this->modelMat = glm::rotate(this->modelMat, angle_deg, axis);
	this->UpdateLocalVectors();
}

void Transform::Scale(glm::vec3 scale)
{
	this->modelMat = glm::scale(modelMat, scale);
}

glm::vec3 Transform::GetPosition()
{
	return this->position;
}

glm::vec3 Transform::GetRotation()
{
	return this->rotation;
}

glm::vec3 Transform::GetScale()
{
	return this->scale;
}

glm::vec3 Transform::GetUp()
{
	return this->up;
}

glm::vec3 Transform::GetFront()
{
	return this->front;
}

glm::vec3 Transform::GetRight()
{
	return this->right;
}

void Transform::SetPosition(glm::vec3 pos)
{
	this->position = pos;
}

void Transform::SetRotation(glm::vec3 rot)
{
	this->rotation = rot;
	this->UpdateLocalVectors();
}

void Transform::SetScale(glm::vec3 scale)
{
	this->scale = scale;
}

glm::mat4 Transform::GetModelMatrix()
{
	return this->modelMat;
}

void Transform::UpdateLocalVectors()
{
	glm::vec3 f;
	f.x = cos(glm::radians(rotation.y)) * cos(glm::radians(rotation.x));
	f.y = sin(glm::radians(rotation.x));
	f.z = sin(glm::radians(rotation.y)) * cos(glm::radians(rotation.x));

	this->front = glm::normalize(f);
	this->right = glm::normalize(glm::cross(front, Transform::UP));
	this->up = glm::normalize(glm::cross(right, front));
}
