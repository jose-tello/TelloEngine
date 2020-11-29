#include "C_Material.h"

#include "Config.h"

#include "Application.h"
#include "M_FileManager.h"
#include "MaterialImporter.h"
#include "M_Resources.h"

#include "R_Texture.h"
#include "R_Material.h"

#include "Glew/include/glew.h"
#pragma comment(lib,"Glew/libx86/glew32.lib")

#include <gl/GL.h>

C_Material::C_Material() : Component(COMPONENT_TYPE::MATERIAL),
	material(nullptr),
	texture(nullptr),

	checkerTexId(0),
	useCheckerTex(false),

	textureEnabled(true),
	colorEnabled(true)
{
	InitCheckerTex();
}


C_Material::~C_Material()
{
	//Desreferentiate
	material = nullptr;
	texture = nullptr;
}


void C_Material::SetTexture(int newTexId)
{
	if (texture != nullptr)
	{
		//Desreferentiate
		texture = nullptr;
	}

	texture = (R_Texture*)App->resourceManager->RequestResource(newTexId);
}


void C_Material::SetMaterial(int newMat)
{
	if (material != nullptr)
	{
		//Desreferentiate
		material = nullptr;
	}

	material = (R_Material*)App->resourceManager->RequestResource(newMat);

	int resourceTexture = material->GetResourceTexture();
	if (resourceTexture != 0)
		texture = (R_Texture*)App->resourceManager->RequestResource(resourceTexture);
}


void C_Material::GetColor(float& r, float& g, float& b, float& a) const
{
	if (material != nullptr)
		material->GetColor(r, g, b, a);
}


std::string C_Material::GetTexturePath() const
{
	if (texture == nullptr)
		return "No texture";

	else
		return texture->GetAssetPath();
}


void C_Material::GetTextureSize(int& width, int& height) const
{
	if (texture == nullptr)
	{
		width = 0;
		height = 0;
	}

	else
		texture->GetTextureSize(width, height);
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
		{
			if (texture != nullptr)
				texId = texture->GetTextureId();
			else
				texId = 0;
		}

		else
			texId = 0;
	}

	if (colorEnabled == true)
		material->GetColor(col.r, col.g, col.b, col.a);

	else
		col = { 1.f, 1.f, 1.f, 1.f };
}


void C_Material::Load(Config& node)
{
	int materialId = node.GetNum("material");
	int textureId = node.GetNum("texture");

	material = (R_Material*)App->resourceManager->RequestResource(materialId);
	texture = (R_Texture*)App->resourceManager->RequestResource(textureId);
}


void C_Material::Save(Config& node) const
{
	node.AppendNum("type", (int)COMPONENT_TYPE::MATERIAL);

	node.AppendNum("material", material->GetUid());
	node.AppendNum("texture", texture->GetUid());
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

	glBindTexture(GL_TEXTURE_2D, 0);
}