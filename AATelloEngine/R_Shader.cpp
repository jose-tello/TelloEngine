#include "R_Shader.h"

#include "ShaderImporter.h"

#include "Application.h"
#include "M_Editor.h"
#include "M_FileManager.h"

#include "UniformHandle.h"

#include <assert.h>

#include "Glew/include/glew.h"
#pragma comment(lib,"Glew/libx86/glew32.lib")

#include <gl/GL.h>


R_Shader::R_Shader(int uid, const char* assetPath, RESOURCE_TYPE type) :
	Resource(uid, assetPath, type),

	programId(0),
	disabled(true)
{
	App->fileManager->SplitPath(assetPath, nullptr, &name, nullptr);
}


R_Shader::~R_Shader()
{
	glDeleteProgram(programId);
	programId = 0;
}


void R_Shader::Load()
{
	ShaderImporter::Load(this);
	isLoaded = true;
}


void R_Shader::UnLoad()
{
	glDeleteProgram(programId);
	programId = 0;
	isLoaded = false;
}


unsigned int R_Shader::GetProgramId() const
{
	return programId;
}


void R_Shader::GetProgramUniforms(std::vector<UniformHandle>& uniformVector) const
{
	int uniformCount = 0;
	glGetProgramiv(programId, GL_ACTIVE_UNIFORMS, &uniformCount);

	char uniformName[256];
	GLenum type;
	int size;

	for (int i = 0; i < uniformCount; i++)
	{
		glGetActiveUniform(programId, i, sizeof(uniformName), nullptr, &size, &type, uniformName);

		std::string name(uniformName);
		VARIABLE_TYPE variableType = (VARIABLE_TYPE)GetUniformType(type);

		if (variableType != VARIABLE_TYPE::NONE)
			uniformVector.push_back(UniformHandle(name, variableType));
		
	}
}


const char* R_Shader::GetShaderName() const
{
	return name.c_str();
}


void R_Shader::SetShaderName(const char* shaderName)
{
	name = shaderName;
}


void R_Shader::InitShader(const char* vertCode, const char* fragCode)
{
	unsigned int vertexShader = 0;
	unsigned int fragmentShader = 0;

	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertCode, NULL);
	glCompileShader(vertexShader);

	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragCode, NULL);
	glCompileShader(fragmentShader);

	if (CheckShadersCompile(vertexShader, fragmentShader) == true)
		CreateShaderProgram(vertexShader, fragmentShader);

	else
		disabled = true;

	if (disabled == false) //If shader program compiled properlly
	{
		glDetachShader(programId, vertexShader);
		glDetachShader(programId, fragmentShader);
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}


void R_Shader::UseShaderProgram() const
{
	glUseProgram(programId);
}


void R_Shader::UnuseShaderProgram() const
{
	glUseProgram(0);
}


void R_Shader::SetBoolUniform(const char* uniformName, bool boolVar)
{
	unsigned int uniformLocation = glGetUniformLocation(programId, uniformName);
	glUniform1i(uniformLocation, boolVar);
}


void R_Shader::SetUintUniform(const char* uniformName, unsigned int uintVar)
{
	unsigned int uniformLocation = glGetUniformLocation(programId, uniformName);
	glUniform1ui(uniformLocation, uintVar);
}


void R_Shader::SetIntUniform(const char* uniformName, int intVar)
{
	unsigned int hasTextureUniform = glGetUniformLocation(programId, uniformName);
	glUniform1i(hasTextureUniform, intVar);
}


void R_Shader::SetFloatUniform(const char* uniformName, float floatVar)
{
	unsigned int timeUniform = glGetUniformLocation(programId, uniformName);
	glUniform1f(timeUniform, floatVar);
}


void R_Shader::CreateShaderProgram(unsigned int vertexShader, unsigned int fragmentShader)
{
	programId = glCreateProgram();
	glAttachShader(programId, vertexShader);
	glAttachShader(programId, fragmentShader);
	glLinkProgram(programId);

	if (CheckProgramCompiles() == true)
		disabled = false;

	else
		disabled = true;
}


bool R_Shader::CheckShadersCompile(unsigned int vertexShader, unsigned int fragmentShader) const
{
	bool ret = true;
	int success = 0;
	char log[512];

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (success == false)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, log);
		App->editor->AddLog("[ERROR] vertex shader error: %s", log);
		ret = false;
	}

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (success == false)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, log);
		App->editor->AddLog("[ERROR] fragment shader error: %s", log);
		ret = false;
	}

	return ret;
}


bool R_Shader::CheckProgramCompiles() const
{
	bool ret = true;
	int success = 0;
	char log[512];

	glGetProgramiv(programId, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(programId, 512, NULL, log);
		App->editor->AddLog("[ERROR] Shader program error: %s", log);
		ret = false;
	}

	return ret;
}


int R_Shader::GetUniformType(unsigned int type) const
{
	switch (type)
	{
	case GL_FLOAT:
		return (int)VARIABLE_TYPE::FLOAT;
		break;

	case GL_FLOAT_VEC2:
		return (int)VARIABLE_TYPE::FLOAT_VEC2;
		break;

	case GL_FLOAT_VEC3:
		return (int)VARIABLE_TYPE::FLOAT_VEC3;
		break;

	case GL_FLOAT_VEC4:
		return (int)VARIABLE_TYPE::FLOAT_VEC4;
		break;

	case GL_INT:
		return (int)VARIABLE_TYPE::INT;
		break;

	case GL_INT_VEC2:
		return (int)VARIABLE_TYPE::INT_VEC2;
		break;

	case GL_INT_VEC3:
		return (int)VARIABLE_TYPE::INT_VEC3;
		break;

	case GL_INT_VEC4:
		return (int)VARIABLE_TYPE::INT_VEC4;
		break;

	case GL_UNSIGNED_INT:
		return (int)VARIABLE_TYPE::UINT;
		break;

	case GL_UNSIGNED_INT_VEC2:
		return (int)VARIABLE_TYPE::UINT_VEC2;
		break;

	case GL_UNSIGNED_INT_VEC3:
		return (int)VARIABLE_TYPE::UINT_VEC3;
		break;

	case GL_UNSIGNED_INT_VEC4:
		return (int)VARIABLE_TYPE::UINT_VEC4;
		break;

	case GL_BOOL:
		return (int)VARIABLE_TYPE::BOOL;
		break;

	case GL_FLOAT_MAT2:
		return (int)VARIABLE_TYPE::FLOAT_MAT2;
		break;

	case GL_FLOAT_MAT3:
		return (int)VARIABLE_TYPE::FLOAT_MAT3;
		break;

	case GL_FLOAT_MAT4:
		return (int)VARIABLE_TYPE::FLOAT_MAT4;
		break;

	default:
		assert("Unsuported uniform type");
		App->editor->AddLog("[WARNING] unsing non supported uniform, will not be displayed nor updated every frame");
		return -1;
		break;
	}
}