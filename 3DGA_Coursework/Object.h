#ifndef OBJECT_H
#define OBJECT_H

#include "Transform.h"
#include "Mesh.h"

class Object
{
public:
	Transform transform;

	virtual void Update(float delta) = 0;
};
#endif // !OBJECT_H
