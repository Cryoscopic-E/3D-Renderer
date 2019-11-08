#ifndef RENDERER_H
#define RENDERER_H

#include "Mesh.h"

class Renderer
{

public:
	Renderer(Mesh& mesh);
	~Renderer();

	void Draw();

private:
	Mesh* mesh;
};

#endif // !RENDERER_H
