#ifndef OBJECT_H
#define OBJECT_H

#include "Transform.h"
#include "Renderer.h"

class Object
{
public:

	void SetParentObject(Object* parent)
	{
		this->parent = parent;
	}

	void AddChildObject(Object* child)
	{
		this->children.push_back(child);
	}

	Object()
	{
		this->parent = this; // initialy an object is parent of himself
		this->transform = new Transform();
	}
	~Object()
	{
		delete this->transform;
		for (int i = 0; i < this->children.size(); i++)
		{
			delete children[i];
		}
	}

	Transform *transform;
	virtual void Update(float delta) = 0;

private:
	Object* parent;
	std::vector<Object*> children;


};
#endif // !OBJECT_H
