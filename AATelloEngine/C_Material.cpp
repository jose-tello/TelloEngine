#include "C_Material.h"

#include "Config.h"

#include "Application.h"
#include "M_Resources.h"

#include "R_Texture.h"
#include "R_Material.h"
#include "R_Shader.h"

#include "UniformHandle.h"

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
	SetShader(App->resourceManager->GetDefaultResource(DEFAULT_RESOURCE::SHADER)->GetUid());
	
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
	{
		shader->AddReference();
		UpdateUniformArray((R_Shader*)shader);
	}

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


UniformHandle* C_Material::GetUniform(const char* name)
{
	std::string nameToSearch(name);

	int uniformCount = shaderUniformsVector.size();
	for (int i = 0; i < uniformCount; i++)
	{
		std::string aux = shaderUniformsVector[i].GetName();

		if (nameToSearch == aux)
			return &shaderUniformsVector[i];
	}

	return nullptr;
}


std::vector<UniformHandle> C_Material::GetUniformVector() const
{
	return shaderUniformsVector;
}


void C_Material::SetUniformVector(std::vector<UniformHandle>& vector)
{
	shaderUniformsVector = vector;
}


void C_Material::SetUniformsToShader() const
{
	unsigned int programId = GetShaderProgram();

	int uniformCount = shaderUniformsVector.size();
	for (int i = 0; i < uniformCount; i++)
	{
		unsigned int uniformLocation = glGetUniformLocation(programId, shaderUniformsVector[i].GetName());

		switch (shaderUniformsVector[i].GetVartiableType())
		{
		case VARIABLE_TYPE::NONE:
			break;


		case VARIABLE_TYPE::BOOL: glUniform1i(uniformLocation, shaderUniformsVector[i].GetBool());	break;


		case VARIABLE_TYPE::UINT: glUniform1ui(uniformLocation, shaderUniformsVector[i].GetUint());	break;


		case VARIABLE_TYPE::UINT_VEC2: 
		{
			unsigned int value[2];
			shaderUniformsVector[i].GetUintVec2(value);

			glUniform2ui(uniformLocation, value[0], value[1]);
		}
			break;


		case VARIABLE_TYPE::UINT_VEC3:
		{
			unsigned int value[3];
			shaderUniformsVector[i].GetUintVec3(value);

			glUniform3ui(uniformLocation, value[0], value[1], value[2]);
		}
		break;


		case VARIABLE_TYPE::UINT_VEC4:
		{
			unsigned int value[4];
			shaderUniformsVector[i].GetUintVec4(value);

			glUniform4ui(uniformLocation, value[0], value[1], value[2], value[3]);
		}
		break;


		case VARIABLE_TYPE::INT: glUniform1i(uniformLocation, shaderUniformsVector[i].GetInt());	break;


		case VARIABLE_TYPE::INT_VEC2:
		{
			int value[2];
			shaderUniformsVector[i].GetIntVec2(value);

			glUniform2i(uniformLocation, value[0], value[1]);
		}
		break;


		case VARIABLE_TYPE::INT_VEC3:
		{
			int value[3];
			shaderUniformsVector[i].GetIntVec3(value);

			glUniform3i(uniformLocation, value[0], value[1], value[2]);
		}
		break;


		case VARIABLE_TYPE::INT_VEC4:
		{
			int value[4];
			shaderUniformsVector[i].GetIntVec4(value);

			glUniform4i(uniformLocation, value[0], value[1], value[2], value[3]);
		}
		
		break;


		case VARIABLE_TYPE::FLOAT: glUniform1f(uniformLocation, shaderUniformsVector[i].GetFloat());	break;


		case VARIABLE_TYPE::FLOAT_VEC2:
		{
			float value[2];
			shaderUniformsVector[i].GetFloatVec2(value);

			glUniform2f(uniformLocation, value[0], value[1]);
		}
		
		break;


		case VARIABLE_TYPE::FLOAT_VEC3:
		{
			float value[3];
			shaderUniformsVector[i].GetFloatVec3(value);

			glUniform3f(uniformLocation, value[0], value[1], value[2]);
		}
		break;


		case VARIABLE_TYPE::FLOAT_VEC4:
		{
			float value[4];
			shaderUniformsVector[i].GetFloatVec4(value);

			glUniform4f(uniformLocation, value[0], value[1], value[2], value[3]);
		}
		break;


		case VARIABLE_TYPE::FLOAT_MAT2:
		{
			float value[4];
			shaderUniformsVector[i].GetMat2(value);

			glUniformMatrix2fv(uniformLocation, 1, GL_FALSE, value);
		}
		break;


		case VARIABLE_TYPE::FLOAT_MAT3:
		{
			float value[9];
			shaderUniformsVector[i].GetMat3(value);

			glUniformMatrix3fv(uniformLocation, 1, GL_FALSE, value);
		}
		break;


		case VARIABLE_TYPE::FLOAT_MAT4:
		{
			float value[16];
			shaderUniformsVector[i].GetMat4(value);

			glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, value);
		}
		break;

		default:
			break;
		}

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
	shaderUniformsVector.clear();

	materialId = node.GetNum("material");
	textureId = node.GetNum("texture");
	shaderId = node.GetNum("shader");

	LoadUniformArray(node);

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

	
	SaveUniformArray(node);
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


void C_Material::UpdateUniformArray(R_Shader* shader)
{
	shaderUniformsVector.clear();

	shader->GetProgramUniforms(shaderUniformsVector);
}


void C_Material::LoadUniformArray(Config& node)
{
	ConfigArray uniformArray = node.GetArray("uniforms");

	int uniformCount = uniformArray.GetSize();

	for (int i = 0; i < uniformCount; i++)
	{
		Config auxNode = uniformArray.GetNode(i);
		
		shaderUniformsVector.push_back(UniformHandle(std::string(auxNode.GetString("name")), (VARIABLE_TYPE)auxNode.GetNum("type")));

		float value[16];

		ConfigArray valueArray = auxNode.GetArray("value_array");
		for (int i = 0; i < 16; i++)
		{
			Config valueNode = valueArray.GetNode(i);
			value[i] = valueNode.GetNum("value");
		}


		shaderUniformsVector[i].SetMat4(value);
	}
}


void C_Material::SaveUniformArray(Config& node) const
{
	ConfigArray arr = node.AppendArray("uniforms");
	int uniformCount = shaderUniformsVector.size();

	for (int i = 0; i < uniformCount; i++)
	{
		Config auxNode = arr.AppendNode();
		auxNode.AppendString("name", shaderUniformsVector[i].GetName());

		auxNode.AppendNum("type", (int)shaderUniformsVector[i].GetVartiableType());

		float value[16];

		shaderUniformsVector[i].GetMat4((float*)value);

		ConfigArray valueArray = auxNode.AppendArray("value_array");

		for (int i = 0; i < 16; i++)
		{
			Config valueNode = valueArray.AppendNode();
			valueNode.AppendNum("value", value[i]);
		}
	}
}