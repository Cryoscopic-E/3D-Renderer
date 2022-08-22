#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>


namespace TextureOptions
{
	enum class FilterType
	{
		NEAREST,
		LINEAR
	};

	enum class WrappingType
	{
		REPEAT,
		CLAMP
	};

	struct TextureSettings
	{
		FilterType filter;
		WrappingType wrapping;
		GLint xOffset;
		GLint yOffset;

		TextureSettings(FilterType f, WrappingType w, GLint xOff, GLint yOff) : filter(f), 
																				wrapping(w),
																				xOffset(xOff),
																				yOffset(yOff){ }
	};
}

class Texture
{
public:
	Texture(const char* texturePath,const TextureOptions::TextureSettings &settings);
	~Texture();

	void bind(const GLint val);
	void unbind();

	GLuint GetTextureID();
private:
	GLuint textureID;
};
#endif // !TEXTURE_HPP
