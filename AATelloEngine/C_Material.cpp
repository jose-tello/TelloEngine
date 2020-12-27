#include "C_Material.h"

#include "Config.h"

#include "Application.h"
#include "M_Resources.h"

#include "R_Texture.h"
#include "R_Material.h"
#include "R_Shader.h"

#include "Glew/include/glew.h"
#pragma comment(lib,"Glew/libx86/glew32.lib")

#include <gl/GL.h>

C_Material::C_Material() : Component(COMPONENT_TYPE::MATERIAL),
	materialId(0),
	textureId(0),
	shaderId(0),

	textureEnabled(true),
	colorEnabled(true),

	checkerTexId(0),
	useCheckerTex(false)
{
	InitCheckerTex();
}


C_Material::~C_Material()
{
	if (materialId != 0)
	{
		Resource* material = App->resourceManager->RequestResource(materialId);

		if (material != nullptr)
			material->QuitReference();
	}

	if (textureId != 0)
	{
		Resource* texture = App->resourceManager->RequestResource(textureId);
		
		if (texture != nullptr)
			texture->QuitReference();
	}

	if (shaderId != 0)
	{
		Resource* shader = App->resourceManager->RequestResource(shaderId);
		
		if (shader != nullptr)
			shader->QuitReference();
	}
}


void C_Material::SetMaterial(int newMat)
{
	Resource* mat = nullptr;

	if (materialId != 0)
	{
		mat = App->resourceManager->RequestResource(materialId);
		if (mat != nullptr)
			mat->QuitReference();
	}

	materialId = newMat;

	mat = App->resourceManager->RequestResource(newMat);
	if (mat != nullptr)
	{
		R_Material* material = (R_Material*)mat;
		material->AddReference();

		SetTexture(material->GetResourceTexture());
		SetShader(material->GetResourceShader());
	}
	else
	{
		SetTexture(0);
		materialId = 0;
		textureId = 0;
		shaderId = 0;
	}
}


void C_Material::SetTexture(int newTexId)
{
	Resource* texture = nullptr;

	if (textureId != 0)
	{
		texture = App->resourceManager->RequestResource(textureId);

		if (texture != nullptr)
			texture->QuitReference();
	}

	textureId = newTexId;

	texture = App->resourceManager->RequestResource(newTexId);

	if (texture != nullptr)
		texture->AddReference();

	else
		textureId = 0;
}


void C_Material::SetShader(int newShader)
{
	Resource* shader = nullptr;

	if (shaderId != 0)
	{
		shader = App->resourceManager->RequestResource(shaderId);

		if (shader != nullptr)
			shader->QuitReference();
	}

	shaderId = newShader;

	shader = App->resourceManager->RequestResource(newShader);

	if (shader != nullptr)
		shader->AddReference();

	else
		shaderId = 0;
}


void C_Material::GetColor(float& r, float& g, float& b, float& a) const
{
	if (materialId != 0)
	{
		Resource* mat = App->resourceManager->RequestResource(materialId);
		if (mat != nullptr)
		{
			R_Material* material = (R_Material*)mat;
			material->GetColor(r, g, b, a);
		}
	}
}


std::string C_Material::GetTexturePath() const
{
	if (textureId == 0)
		return "No texture";

	else
	{
		Resource* tex = App->resourceManager->RequestResource(textureId);
		if (tex != nullptr)
		{
			R_Texture* texture = (R_Texture*)tex;
			return texture->GetAssetPath();
		}
		else
			return "No texture";
	}
}


void C_Material::GetTextureSize(int& width, int& height) const
{
	width = 0;
	height = 0;

	if (textureId != 0)
	{
		Resource* tex = App->resourceManager->RequestResource(textureId);
		if (tex != nullptr)
		{
			R_Texture* texture = (R_Texture*)tex;
			texture->GetTextureSize(width, height);
		}
	}
}


int C_Material::GetShader() const
{
	return shaderId;
}


std::string C_Material::GetShaderName() const
{
	if (shaderId == 0)
		return "No shader";

	else
	{
		Resource* shaderResource = App->resourceManager->RequestResource(shaderId);
		if (shaderResource != nullptr)
		{
			R_Shader* shader = (R_Shader*)shaderResource;
			return shader->GetShaderName();
		}
		else
			return "No shader";
	}
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


void C_Material::GetDrawVariables(Color& col, unsigned int& texId, unsigned int& shaderProgramId) const
{
	GetDrawColor(col);
	texId = GetTextureId();
	shaderProgramId = GetShaderProgram();
}


void C_Material::Load(Config& node)
{
	materialId = node.GetNum("material");
	textureId = node.GetNum("texture");
	shaderId = node.GetNum("shader");

	if (materialId != 0)
	{
		Resource* material = App->resourceManager->RequestResource(materialId);

		if (material != nullptr)
			material->AddReference();
	}

	if (textureId != 0)
	{
		Resource* texture = App->resourceManager->RequestResource(textureId);
		if (texture != nullptr)
			texture->AddReference();
	}

	if (shaderId != 0)
	{
		Resource* shader = App->resourceManager->RequestResource(shaderId);
		if (shader != nullptr)
			shader->AddReference();
	}
}


void C_Material::Save(Config& node) const
{
	node.AppendNum("type", (int)COMPONENT_TYPE::MATERIAL);

	node.AppendNum("material", materialId);
	node.AppendNum("texture", textureId);
	node.AppendNum("shader", shaderId);
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


void C_Material::GetDrawColor(Color& col) const
{
	if (colorEnabled == true)
	{
		if (materialId != 0)
		{
			Resource* mat = App->resourceManager->RequestResource(materialId);
			if (mat != nullptr)
			{
				R_Material* material = (R_Material*)mat;
				material->GetColor(col.r, col.g, col.b, col.a);
				return;
			}
		}
	}

	col = { 1.f, 1.f, 1.f, 1.f };
}


unsigned int C_Material::GetTextureId() const
{
	if (useCheckerTex == true)
		return checkerTexId;

	else
	{
		if (textureEnabled == true)
		{
			if (textureId != 0)
			{
				Resource* tex = App->resourceManager->RequestResource(textureId);
				if (tex != nullptr)
				{
					R_Texture* texture = (R_Texture*)tex;
					return texture->GetTextureId();
				}
			}
		}
	}

	return 0;
}


unsigned int C_Material::GetShaderProgram() const
{
	if (shaderId != 0)
	{
		Resource* resourceShader = App->resourceManager->RequestResource(shaderId);
		if (resourceShader != nullptr)
		{
			R_Shader* shader = (R_Shader*)resourceShader;
			return shader->GetProgramId();
		}
	}

	else
	{
		R_Shader* shader = (R_Shader*)App->resourceManager->GetDefaultResource(DEFAULT_RESOURCE::SHADER);
		return shader->GetProgramId();
	}
}