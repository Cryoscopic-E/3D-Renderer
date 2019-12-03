#ifndef RENDERER_H
#define RENDERER_H

#include "Mesh.h"
#include "Material.h"
class Renderer
{

public:
	Renderer(Mesh* mesh, Material * mat);

	void prepareShader();
	void detachShader();
	void drawMesh();
	Shader* GetShader();
	Material* material;
private:
	Mesh* mesh;
	
};

#endif // !RENDERER_H
