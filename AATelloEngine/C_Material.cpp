#include "C_Material.h"

#include "Glew/include/glew.h"
#pragma comment(lib,"Glew/libx86/glew32.lib")

#include <gl/GL.h>

C_Material::C_Material() : Component(COMPONENT_TYPE::MATERIAL),
	textureId(0),
	color(1.f, 1.f, 1.f),

	texturePath(),
	textureName(),
	textureWidth(0),
	textureHeight(0),

	checkerTexId(0),
	useCheckerTex(false),

	textureEnabled(true),
	colorEnabled(true)
{
	InitCheckerTex();
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

	InitTextureSize();
}


void C_Material::GetColor(float& r, float& g, float& b, float& a) const
{
	r = color.r;
	g = color.g;
	b = color.b;
	a = color.a;
}


void C_Material::SetColor(Color& col)
{
	color = col;
}


bool C_Material::GetTextureEnabled() const
{
	return textureEnabled;
}


void C_Material::SetTextureEnable(bool enable)
{
	if (textureEnabled != enable)
		textureEnabled = enable;
}


bool C_Material::GetColorEnabled() const
{
	return colorEnabled;
}


void C_Material::SetColorEnable(bool enable)
{
	if (colorEnabled != enable)
		colorEnabled = enable;
}


bool C_Material::GetCheckerTextureEnabled() const
{
	return useCheckerTex;
}


void C_Material::SetCheckerTextureEnable(bool enable)
{
	if (useCheckerTex != enable)
		useCheckerTex = enable;
}


void C_Material::GetDrawVariables(unsigned int& texId, Color& col) const
{
	if (useCheckerTex == true)
		texId = checkerTexId;

	else
	{
		if (textureEnabled == true)
			texId = textureId;

		else
			texId = 0;
	}

	if (colorEnabled == true)
		col = color;

	else
		col = { 1.f, 1.f, 1.f, 1.f };
}


void C_Material::InitTextureSize()
{
	glBindTexture(GL_TEXTURE_2D, textureId);

	int miplevel = 0;
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &textureWidth);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &textureHeight);

	glBindTexture(GL_TEXTURE_2D, 0);
}


void C_Material::InitCheckerTex()
{
	GLubyte checkerImage[CHECKERS_HEIGHT][CHECKERS_WIDTH][4];
	for (int i = 0; i < CHECKERS_HEIGHT; i++) {
		for (int j = 0; j < CHECKERS_WIDTH; j++) {
			int c = ((((i & 0x8) == 0) ^ (((j & 0x8)) == 0))) * 255;
			checkerImage[i][j][0] = (GLubyte)c;
			checkerImage[i][j][1] = (GLubyte)c;
			checkerImage[i][j][2] = (GLubyte)c;
			checkerImage[i][j][3] = (GLubyte)255;
		}
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &checkerTexId);
	glBindTexture(GL_TEXTURE_2D, checkerTexId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, CHECKERS_WIDTH, CHECKERS_HEIGHT,
		0, GL_RGBA, GL_UNSIGNED_BYTE, checkerImage);
}