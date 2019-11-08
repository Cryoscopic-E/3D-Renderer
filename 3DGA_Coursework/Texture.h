#ifndef TEXTURE_H
#define TEXTURE_H

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

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

	void Use();
private:
	GLuint texture;
};
#endif // !TEXTURE_H
