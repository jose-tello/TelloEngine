#include "ShaderImporter.h"

#include "Application.h"
#include "M_FileManager.h"
#include "M_Editor.h"

#include "R_Shader.h"

void ShaderImporter::Import(const char* path, R_Shader* shader)
{
	std::string filePath(path);

	char* buffer = nullptr;
	unsigned int bytes = App->fileManager->ReadBytes(filePath.c_str(), &buffer);

	std::string programCode(buffer);
	const size_t vertexIterator = programCode.find(VERTEX_SHADER_KEY);
	const size_t fragmentIterator = programCode.find(FRAGMENT_SHADER_KEY);

	if (vertexIterator != std::string::npos && fragmentIterator != std::string::npos)
	{
		std::string vertexCode = programCode.substr(vertexIterator + VERTEX_KEY_LENGHT, fragmentIterator);
		std::string fragmentCode = programCode.substr(fragmentIterator + FRAGMENT_KEY_LENGHT);

		shader->InitShader(vertexCode.c_str(), fragmentCode.c_str());
		Save(shader);

		App->editor->AddLog("Log: Imported shader from: %s", path);
	}
	else
	{
		App->editor->AddLog("[ERROR] Could not find vertex/fragment key words of shader from: %s", path);
	}
	
	//TODO: Should unload the resource?
}


void ShaderImporter::Load(R_Shader* shader)
{
	char* fileBuffer = nullptr;

	std::string path(SHADER_LIBRARY);
	path.append(std::to_string(shader->GetUid()));

	unsigned int size = App->fileManager->Load(path.c_str(), &fileBuffer);

	if (size > 0)
	{
		char* programCode = new char[size / sizeof(char)];
		memcpy(programCode, fileBuffer, size);

		shader->InitShaderFromBinary(programCode, size);
	}
	else
		App->editor->AddLog("[WARNING], shader not loaded correctlly from: %s", path.c_str());

	delete[] fileBuffer;
	fileBuffer = nullptr;
}


void ShaderImporter::Save(const R_Shader* shader)
{
	std::string filePath(SHADER_LIBRARY);
	filePath.append(std::to_string(shader->GetUid()));

	std::string shaderProgramCode = shader->GetProgramCode();

	unsigned int size = shaderProgramCode.length() * sizeof(char);
	
	char* fileBuffer = new char[size];

	unsigned int bytes = shaderProgramCode.length() * sizeof(char);
	memcpy(fileBuffer, shaderProgramCode.c_str(), bytes);
	
	App->fileManager->Save(filePath.c_str(), fileBuffer, size);

	delete[] fileBuffer;
	fileBuffer = nullptr;
}


void ShaderImporter::SaveAsAsset(const R_Shader* shader)
{
	//TODO: ill figure it out later
}