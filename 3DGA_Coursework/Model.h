#ifndef MODEL_H
#define MODEL_H

#include "Object.h"
#include "Renderer.h"
#include "Camera.h"
class Model : public Object
{
public:
	Model(Renderer* renderer, Camera* camera);

	void Render();
	void Update(float delta);
private:
	Camera* camera;
	Renderer* renderer;
};
#endif // !MODEL_H
