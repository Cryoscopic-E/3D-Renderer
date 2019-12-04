#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
#include <sstream>
#include <string>
#include <fstream>

class Shader
{
public:
	GLuint program;

	Shader(const char* vertexPath, const char* fragmentPath);
	~Shader();

	void bind();
	void unbind();

	void setUniform(const std::string name, bool val);
	void setUniform(const std::string name, int val);
	void setUniform(const std::string name, float val);
	void setUniform(const std::string name, glm::vec3 val);
	void setUniform(const std::string name, glm::mat4 val);
private:
	void loadShaders();
	void checkCompilationError(GLuint shader);
	void checkProgramLinkError();

	GLint getUniformLocation(const char* name);

	GLchar log[512];
	const GLchar* fsPath;
	const GLchar* vsPath;
	const GLchar* fsCode;
	const GLchar* vsCode;
};
#endif // !SHADER_H
