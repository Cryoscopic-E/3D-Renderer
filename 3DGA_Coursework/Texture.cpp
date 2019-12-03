#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>
Texture::Texture(const char* texturePath, const TextureOptions::TextureSettings& settings)
{
	glGenTextures(1,&this->textureID);
	stbi_set_flip_vertically_on_load(true);
	int width, height, channels;
	unsigned char* data = stbi_load(texturePath, &width, &height, &channels, 0);
	glBindTexture(GL_TEXTURE_2D, this->textureID);

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
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		break;
	case TextureOptions::FilterType::NEAREST:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Error loading texture" << std::endl;
	}
	//glTexSubImage2D(GL_TEXTURE_2D, 0, settings.xOffset, settings.yOffset, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);
	stbi_image_free(data);
}

Texture::~Texture()
{
	glDeleteTextures(1, &textureID);
}

void Texture::bind(const GLint val)
{
	glActiveTexture(GL_TEXTURE0 + val);
	glBindTexture(GL_TEXTURE_2D, this->textureID);
}

void Texture::unbind()
{
	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_2D,0);
}

GLuint Texture::GetTextureID()
{
	return this->textureID;
}

