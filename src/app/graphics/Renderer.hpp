#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "Mesh.hpp"
#include "Material.hpp"

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

#endif // !RENDERER_HPP
