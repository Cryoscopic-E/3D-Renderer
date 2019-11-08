#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
Texture::Texture(const char* texturePath, const TextureOptions::TextureSettings& settings)
{
	glGenTextures(1,&texture);
	int width, height, channels;
	unsigned char* data = stbi_load(texturePath, &width, &height, &channels, 0);
	glBindTexture(1, texture);

	switch (settings.wrapping)
	{
	case TextureOptions::WrappingType::REPEAT:
	default:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		break;
	case TextureOptions::WrappingType::CLAMP:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	}

	switch (settings.filter)
	{
	case TextureOptions::FilterType::LINEAR:
	default:
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		break;
	case TextureOptions::FilterType::NEAREST:
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}

	glTexSubImage2D(GL_TEXTURE_2D, 0, settings.xOffset, settings.yOffset, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);
	stbi_image_free(data);
}

Texture::~Texture()
{
	glDeleteTextures(1, &texture);
}

void Texture::Use()
{
	glBindTexture(GL_TEXTURE_2D, texture);
}
