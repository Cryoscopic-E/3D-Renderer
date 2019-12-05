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


private:

};
#endif // !OBJECT_H
