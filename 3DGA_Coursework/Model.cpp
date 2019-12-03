#include "Model.h"

Model::Model(Renderer* renderer, Camera* camera)
{
	this->renderer = renderer;
	this->camera = camera;
}


void Model::Render()
{
	this->renderer->prepareShader();
	glUniformMatrix4fv(glGetUniformLocation(this->renderer->GetShader()->program, "model_matrix"), 1, GL_FALSE, &this->transform->GetModelMatrix()[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(this->renderer->GetShader()->program, "view_matrix"), 1, GL_FALSE, &this->camera->GetViewMat()[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(this->renderer->GetShader()->program, "proj_matrix"), 1, GL_FALSE, &this->camera->GetProjectionMat()[0][0]);
	this->renderer->GetShader()->setUniform("viewPosition", this->camera->transform->GetPosition());
	this->renderer->drawMesh();
	this->renderer->detachShader();
}

void Model::Update(float delta)
{
}
