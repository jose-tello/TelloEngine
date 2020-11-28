#include "R_Material.h"
#include "MaterialImporter.h"

#include "Glew/include/glew.h"
#pragma comment(lib,"Glew/libx86/glew32.lib")

#include <gl/GL.h>

R_Material::R_Material(int uid, const char* assetPath, RESOURCE_TYPE type) : Resource(uid, assetPath, type),
	textureWidth(0),
	textureHeight(0),
	textureId(0),
	color(1.f, 1.f, 1.f)
{
}


R_Material::~R_Material()
{
	glDeleteTextures(1, &textureId);
	textureId = 0;
}


void R_Material::Load()
{
	//MaterialImporter::Load(this, uid);
}


void R_Material::UnLoad()
{
	glDeleteTextures(1, &textureId);
	textureId = 0;
}


void R_Material::SetTexture(unsigned int newTex)
{
	if (textureId != 0)
	{
		glDeleteTextures(1, &textureId);
		textureId = 0;
	}

	textureId = newTex;

	InitTextureSize();
}


void R_Material::SetColor(Color& col)
{
	color = col;
}


void R_Material::GetDrawVariables(unsigned int& texId, Color& col) const
{
	texId = textureId;
	col = color;
}


void R_Material::InitTextureSize()
{
	glBindTexture(GL_TEXTURE_2D, textureId);

	int miplevel = 0;
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &textureWidth);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &textureHeight);

	glBindTexture(GL_TEXTURE_2D, 0);
}