#include "C_Material.h"

#include "Glew/include/glew.h"
#pragma comment(lib,"Glew/libx86/glew32.lib")

#include <gl/GL.h>

C_Material::C_Material(GameObject* owner) : Component(COMPONENT_TYPE::MATERIAL, owner),
	textureId(0),
	color(),

	textureEnabled(true),
	colorEnabled(true)
{
}


C_Material::~C_Material()
{
	glDeleteTextures(1, &textureId);
	textureId = 0;
}


void C_Material::SetTexture(unsigned int newTex)
{
	if (textureId != 0)
	{
		glDeleteTextures(1, &textureId);
		textureId = 0;
	}

	textureId = newTex;
}


void C_Material::SetColor(Color& col)
{
	color = col;
}


void C_Material::DeleteTexture()
{
	glDeleteTextures(1, &textureId);
	textureId = 0;
}


void C_Material::DeleteColor()
{
	color = { 0.f, 0.f, 0.f, 0.f };
}


void C_Material::SetTextureEnable(bool enable)
{
	if (textureEnabled != enable)
		textureEnabled = enable;
}


void C_Material::SetColorEnable(bool enable)
{
	if (colorEnabled != enable)
		colorEnabled = enable;
}


void C_Material::GetDrawVariables(unsigned int& texId, Color& col) const
{
	if (textureEnabled == true)
		texId = textureId;

	else
		texId = 0;
	

	if (colorEnabled == true)
		col = color;

	else
		col = { 0.f, 0.f, 0.f, 0.f };
}