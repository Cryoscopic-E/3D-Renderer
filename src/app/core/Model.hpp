#ifndef MODEL_HPP
#define MODEL_HPP

#include "Object.hpp"
#include "graphics/Renderer.hpp"
#include "graphics/Camera.hpp"

class Model : public Object
{
public:
	Model(Renderer* renderer, Camera* camera);
	~Model();
	void Render();
	void Update(float delta);
private:
	Camera* camera;
	Renderer* renderer;
};
#endif // !MODEL_HPP
