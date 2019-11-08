#include "Shader.h"
#include <iostream>

Shader::Shader(const char* vertex, const char* fragment)
{
	loadShaders(vertex, fragment);
}

Shader::~Shader()
{
	glDeleteProgram(program);
}

void Shader::use()
{
	glUseProgram(program);
}

void Shader::setUniform(const char* name, bool val)
{
	glUniform1i(getUniformLocation(name), val);
}

void Shader::setUniform(const char* name, int val)
{
	glUniform1i(getUniformLocation(name), val);
}

void Shader::setUniform(const char* name, float val)
{
	glUniform1f(getUniformLocation(name), val);
}

void Shader::setUniform(const char* name, glm::vec3 val)
{
	glUniform3f(getUniformLocation(name), val.x, val.y, val.z);
}

void Shader::setUniform(const char* name, glm::mat4 val)
{
	glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &val[0][0]);
}

void Shader::loadShaders(const char* vShader, const char* fShader)
{
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vShader, NULL);
	glCompileShader(vertexShader);
	checkCompilationError(vertexShader);

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fShader, NULL);
	glCompileShader(fragmentShader);
	checkCompilationError(fragmentShader);

	program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);
	checkProgramLinkError();

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Shader::checkCompilationError(GLuint shader)
{
	int ok;
	char log[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &ok);
	if (!ok)
	{
		glGetShaderInfoLog(shader, 512,NULL, log);
		std::cout << "error compiling shader " << log << std::endl;
	}
}

void Shader::checkProgramLinkError()
{
	int ok;
	char log[512];
	glGetProgramiv(program, GL_LINK_STATUS, &ok);
	if (!ok)
	{
		glGetProgramInfoLog(program, 512, NULL, log);
		std::cout << "error linking shader program " << log << std::endl;
	}
}

GLint Shader::getUniformLocation(const char* name)
{
	return glGetUniformLocation(program, name);
}
