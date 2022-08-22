#include "Camera.hpp"
#include <glm/gtx/transform.hpp>



void Camera::Update(float delta)
{
	//TODO listen for inputs
}

glm::mat4 Camera::GetViewMat()
{
	// For now only look at center 0,0,0
	return glm::lookAt(transform->GetPosition(), transform->GetPosition() + transform->GetFront(), transform->GetUp());
}

glm::mat4 Camera::GetProjectionMat()
{
	return projectionMat;
}

Camera::Camera(int windowWidth, int windowHeight, float fov, float near, float far)
{
	this->fieldOfView = fov;
	this->near = near;
	this->far = far;
	this->aspectRatio = (float)windowWidth / (float)windowHeight;
	
	this->projectionMat = glm::perspective(glm::radians(fieldOfView), aspectRatio, near, far);
}

Camera::~Camera()
{
}

void Camera::UpdateView(float w, float h)
{
	this->aspectRatio = (float)w / (float)h;

	this->projectionMat = glm::perspective(glm::radians(fieldOfView), aspectRatio, near, far);
}

