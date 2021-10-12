#include "ShaderImporter.h"

#include "Application.h"
#include "M_FileManager.h"
#include "M_Editor.h"
#include "M_Resources.h"

#include "R_Shader.h"

void ShaderImporter::Import(const char* path, R_Shader* shader)
{
	std::string filePath(path);

	char* buffer = nullptr;
	unsigned int bytes = App->fileManager->ReadBytes(filePath.c_str(), &buffer);

	std::string programCode(buffer);
	programCode = programCode.substr(0, programCode.find_last_of("}") + 1);
	const size_t vertexIterator = programCode.find(VERTEX_SHADER_KEY);
	const size_t fragmentIterator = programCode.find(FRAGMENT_SHADER_KEY);
	const size_t computeIterator = programCode.find(COMPUTE_SHADER_KEY);

	if (vertexIterator != std::string::npos || fragmentIterator != std::string::npos || computeIterator != std::string::npos)
	{
		std::string vertexCode = "";
		std::string fragmentCode = "";
		std::string computeCode = "";

		if (vertexIterator != std::string::npos)
		{
			vertexCode = programCode.substr(vertexIterator + VERTEX_KEY_LENGHT, fragmentIterator - FRAGMENT_KEY_LENGHT);
		}

		if (fragmentIterator != std::string::npos)
		{
			fragmentCode = programCode.substr(fragmentIterator + FRAGMENT_KEY_LENGHT, computeIterator - COMPUTE_KEY_LENGHT);
		}

		if (computeIterator != std::string::npos)
		{
			computeCode = programCode.substr(computeIterator + COMPUTE_KEY_LENGHT);
		}

		shader->InitShaderProgram(vertexCode, fragmentCode, computeCode);
		Save(programCode, shader->GetUid());

		App->editor->AddLog("Log: Imported shader from: %s", path);
	}
	else
		App->editor->AddLog("[ERROR] Could not find vertex & fragment or compute key words of shader from: %s", path);
	

	delete[] buffer;
	buffer = nullptr;
	shader->UnLoad();
}


void ShaderImporter::Load(R_Shader* shader)
{
	char* fileBuffer = nullptr;

	std::string path(SHADER_LIBRARY);
	path.append(std::to_string(shader->GetUid()));

	unsigned int size = App->fileManager->Load(path.c_str(), &fileBuffer);

	if (size > 0)
	{
		std::string code(fileBuffer);
		code = code.substr(0, code.find_last_of("}") + 1);

		const size_t vertexIterator = code.find(VERTEX_SHADER_KEY);
		const size_t fragmentIterator = code.find(FRAGMENT_SHADER_KEY);
		const size_t computeIterator = code.find(COMPUTE_SHADER_KEY);

		std::string vertexCode = "";
		std::string fragmentCode = "";
		std::string computeCode = "";

		if (vertexIterator != std::string::npos)
		{
			vertexCode = code.substr(vertexIterator + VERTEX_KEY_LENGHT, fragmentIterator - FRAGMENT_KEY_LENGHT);
		}

		if (fragmentIterator != std::string::npos)
		{
			fragmentCode = code.substr(fragmentIterator + FRAGMENT_KEY_LENGHT, computeIterator - COMPUTE_KEY_LENGHT);
		}

		if (computeIterator != std::string::npos)
		{
			computeCode = code.substr(computeIterator + COMPUTE_KEY_LENGHT);
		}

		shader->InitShaderProgram(vertexCode, fragmentCode, computeCode);
	}
	else
		App->editor->AddLog("[WARNING], shader not loaded correctlly from: %s", path.c_str());

	delete[] fileBuffer;
	fileBuffer = nullptr;
}


void ShaderImporter::Save(std::string& shaderCode, int uid)
{
	std::string filePath(SHADER_LIBRARY);
	filePath.append(std::to_string(uid));

	unsigned int size = shaderCode.length() * sizeof(char);
	
	char* fileBuffer = new char[size];

	memcpy(fileBuffer, shaderCode.c_str(), size);
	
	App->fileManager->Save(filePath.c_str(), fileBuffer, size);

	delete[] fileBuffer;
	fileBuffer = nullptr;
}


void ShaderImporter::SaveAsAsset(int shaderId, const char* code)
{
	Resource* shader = App->resourceManager->RequestResource(shaderId);
	std::string filePath = shader->GetAssetPath();

	std::string shaderCode(code);

	unsigned int size = shaderCode.length() * sizeof(char);

	char* fileBuffer = new char[size];

	unsigned int bytes = shaderCode.length() * sizeof(char);
	memcpy(fileBuffer, shaderCode.c_str(), bytes);

	App->fileManager->Save(filePath.c_str(), fileBuffer, size);

	delete[] fileBuffer;
	fileBuffer = nullptr;

	App->resourceManager->UpdateAllAssets();

}