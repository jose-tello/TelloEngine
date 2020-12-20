#include "R_Shader.h"

#include "ShaderImporter.h"

#include "Application.h"
#include "M_Editor.h"
#include "M_FileManager.h"

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

}


void R_Shader::UnLoad()
{
	/*glDeleteProgram(programId);
	programId = 0;*/
}


void R_Shader::Save() const
{
	ShaderImporter::Save(this);
}


void R_Shader::Rewrite(const char* file)
{
	//TODO: I have to think about this one
}


unsigned int R_Shader::GetProgramId() const
{
	return programId;
}


const char* R_Shader::GetProgramCode() const
{
	char* code = nullptr;
	
	int binaryLenght = 0;
	glGetProgramiv(programId, GL_PROGRAM_BINARY_LENGTH, &binaryLenght);

	if (binaryLenght > 0)
	{
		code = new char[binaryLenght];
		GLenum format;
		GLsizei writtenLength = 0;
		glGetProgramBinary(programId, binaryLenght, &writtenLength, &format, code);

		return code;
	}

	glUseProgram(0);
	return code;
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

	if (disabled == false)	//TODO: Test this thing works properlly
	{
		glDetachShader(programId, vertexShader);
		glDetachShader(programId, fragmentShader);
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}


void R_Shader::InitShaderFromBinary(const char* programCode, unsigned int size)
{
	programId = glCreateProgram();
	glProgramBinary(programId, 36385, programCode, size);
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