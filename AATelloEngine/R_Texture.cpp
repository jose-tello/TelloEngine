#include "R_Texture.h"

#include "TextureImporter.h"

#include "Glew/include/glew.h"
#pragma comment(lib,"Glew/libx86/glew32.lib")

#include <gl/GL.h>

R_Texture::R_Texture(int uid, const char* assetPath, RESOURCE_TYPE type) : Resource(uid, assetPath, (RESOURCE_TYPE)type),
	textureId(0),
	textureWidth(0),
	textureHeight(0)
{
}


R_Texture::~R_Texture()
{
	glDeleteTextures(1, &textureId);
	textureId = 0;
}


void R_Texture::Load()
{
	TextureImporter::Load(this);
	InitTextureSize();
}


void R_Texture::UnLoad()
{
	glDeleteTextures(1, &textureId);
	textureId = 0;
}


unsigned int R_Texture::GetTextureId() const
{
	return textureId;
}


void R_Texture::SetTextureId(unsigned int texId)
{
	textureId = texId;
}


void R_Texture::GetTextureSize(int& w, int& h) const
{
	w = textureWidth;
	h = textureHeight;
}


void R_Texture::InitTextureSize()
{
	glBindTexture(GL_TEXTURE_2D, textureId);

	int miplevel = 0;
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &textureWidth);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &textureHeight);

	glBindTexture(GL_TEXTURE_2D, 0);
}