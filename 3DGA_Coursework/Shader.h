#ifndef SHADER_H
#define SHADER_H

#include <GLFW/glfw3.h>
#include <GL/glew.h>
#include <glm/glm.hpp>

class Shader
{
public:
	GLuint program;

	Shader(const char* vertexPath, const char* fragmentPath);
	~Shader();

	void use();

	void setUniform(const char* name, bool val);
	void setUniform(const char* name, int val);
	void setUniform(const char* name, float val);
	void setUniform(const char* name, glm::vec3 val);
	void setUniform(const char* name, glm::mat4 val);
private:
	void loadShaders(const char* vShader, const char* fShader);
	void checkCompilationError(GLuint shader);
	void checkProgramLinkError();

	GLint getUniformLocation(const char* name);
};
#endif // !SHADER_H
