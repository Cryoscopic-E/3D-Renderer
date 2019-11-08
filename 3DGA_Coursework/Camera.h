#ifndef CAMERA_H
#define CAMERA_H

#include "Object.h"

class Camera : public Object
{
public: 

	
	float fieldOfView;
	float near;
	float far;
	float aspectRatio;

	void Update(float delta);

	glm::mat4 GetViewMat();
	glm::mat4 GetProjectionMat();

	Camera(int windowWidth, int windowHeight, float fov, float near, float far);
	~Camera();

	void UpdateView(float w, float h);
private:
	glm::mat4 projectionMat;
};
#endif // !CAMERA_H
