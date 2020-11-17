#include "MaterialImporter.h"
#include "TextureImporter.h"

#include "Application.h"
#include "M_FileManager.h"
#include "M_Editor.h"

#include "C_Material.h"

#include "Assimp/include/material.h"


void MaterialImporter::Import(aiMaterial* material, C_Material* materialComponent, Color& color, bool hasTexture, 
							  bool hasColor, const char* nodeName)
{
	std::string materialName(nodeName);

	if (hasTexture)
	{
		aiString texPath;
		material->GetTexture(aiTextureType::aiTextureType_DIFFUSE, 0, &texPath);

		std::string fileName;
		std::string fileExtension;
		App->fileManager->SplitPath(texPath.C_Str(), nullptr, &materialName, &fileExtension);
		fileName = materialName + "." + fileExtension;

		materialComponent->SetTexture(TextureImporter::Import(fileName.c_str(), false));
		materialComponent->texturePath = texPath.C_Str();
		materialComponent->textureName = materialName;
	}


	if (hasColor)
		materialComponent->SetColor(color);

	materialComponent->materialPath = materialName;

	std::string materialPath = Save(materialComponent, materialName.c_str());
	Load(materialComponent, materialPath.c_str());
}


void MaterialImporter::Load(C_Material* material, const char* path)
{
	char* fileBuffer = nullptr;
	unsigned int size = App->fileManager->Load(path, &fileBuffer);
	
	char* pointer = fileBuffer;
	if (pointer != nullptr)
	{
		float color[4];
		unsigned int bytes = sizeof(color);
		memcpy(color, pointer, bytes);
		pointer += bytes;

		material->SetColor(Color(color[0], color[1], color[2], color[3]));

		if (bytes < size)
		{		
			std::string filePath(TEXTURE_LIBRARY);
			filePath.append(pointer);	//Name of the file
			material->SetTexture(TextureImporter::Load(filePath.c_str()));
			material->textureName = pointer;	
		}
	}

	delete[] fileBuffer;
	fileBuffer = nullptr;

	App->editor->AddLog("Log: Loaded material: %s", material->textureName.c_str());
}


//The string returned is the path to the mesh
std::string MaterialImporter::Save(C_Material* materialComponent, const char* fileName)
{
	std::string filePath(MATERIAL_LIBRARY);
	filePath.append(fileName);

	float color[4];
	materialComponent->GetColor(color[0], color[1], color[2], color[3]);

	unsigned int size = sizeof(color) + sizeof(char) * materialComponent->textureName.length();
	char* fileBuffer = new char[size];
	char* pointer = fileBuffer;

	//Save color
	unsigned int bytes = sizeof(color);
	memcpy(pointer, color, bytes);
	pointer += bytes;

	//Save texture name
	if (materialComponent->textureName.length() != 0)
	{
		bytes = sizeof(char) * materialComponent->textureName.length();
		const char* texName = materialComponent->textureName.c_str();
		memcpy(pointer, texName, bytes);
	}
	
	App->fileManager->Save(filePath.c_str(), fileBuffer, size);
	App->editor->AddLog("Log: Saved material %s: ", fileName);
	
	delete[] fileBuffer;
	fileBuffer = nullptr;
	
	return filePath;
}