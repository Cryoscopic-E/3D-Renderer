#include "Renderer.h"

Renderer::Renderer(Mesh* mesh, Material* mat)
{
	this->mesh = mesh;
	this->material = mat;
}

void Renderer::prepareShader()
{
	material->use();	
}
void Renderer::detachShader()
{
	material->unuse();
}
void Renderer::drawMesh()
{
	mesh->Draw();
}

Shader* Renderer::GetShader()
{
	return this->material->GetShader();
}
