#ifndef OBJECT_H
#define OBJECT_H

#include "Transform.h"
#include "Renderer.h"

class Object
{
public:

	Object()
	{
		this->transform = new Transform();
	}
	~Object()
	{
		delete this->transform;
	}

	Transform *transform;
	virtual void Update(float delta) = 0;
};
#endif // !OBJECT_H
