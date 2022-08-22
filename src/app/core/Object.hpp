#ifndef OBJECT_HPP
#define OBJECT_HPP

#include "Transform.hpp"
#include "graphics/Renderer.hpp"

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
#endif // !OBJECT_HPP
