#include "MaterialImporter.h"

#include "Application.h"
#include "M_FileManager.h"
#include "M_Editor.h"

#include "M_Resources.h"
#include "R_Material.h"

#include "Assimp/include/material.h"
#include "MathGeoLib/src/Algorithm/Random/LCG.h"

void MaterialImporter::Import(aiMaterial* material, Color& color, bool hasTexture, bool hasColor, const char* assetPath)
{
	LCG random;

	R_Material* materialResource = new R_Material(random.IntFast(), assetPath, RESOURCE_TYPE::MATERIAL);
	if (hasTexture)
	{
		aiString texPath;
		material->GetTexture(aiTextureType::aiTextureType_DIFFUSE, 0, &texPath);

		std::string fileName;
		std::string fileExtension;
		App->fileManager->SplitPath(texPath.C_Str(), nullptr, &fileName, &fileExtension);
		fileName = fileName + "." + fileExtension;
		int id = App->resourceManager->SearchMetaFile(fileName.c_str());

		if (id == 0)
			materialResource->SetResourceTexture(App->resourceManager->CreateMeta(fileName.c_str()));

		else
			materialResource->SetResourceTexture(id);
	}

	if (hasColor)
		materialResource->SetColor(color);

	Save(materialResource);

	App->resourceManager->PushResource(materialResource, materialResource->GetUid());
}


void MaterialImporter::Load(R_Material* material)
{
	char* fileBuffer = nullptr;
	std::string path(MATERIAL_LIBRARY);
	path.append(std::to_string(material->GetUid()));
	unsigned int size = App->fileManager->Load(path.c_str(), &fileBuffer);
	
	char* pointer = fileBuffer;
	if (pointer != nullptr)
	{
		float color[4];
		unsigned int bytes = sizeof(color);
		memcpy(color, pointer, bytes);
		pointer += bytes;

		material->SetColor(Color(color[0], color[1], color[2], color[3]));

		int texId;
		memcpy(&texId, pointer, sizeof(int));

		material->SetResourceTexture(texId);
	}

	delete[] fileBuffer;
	fileBuffer = nullptr;

	App->editor->AddLog("Log: Loaded material: %s", material->GetAssetPath());
}


//The string returned is the path to the mesh
std::string MaterialImporter::Save(R_Material* materialResource)
{
	std::string filePath(MATERIAL_LIBRARY);
	filePath.append(std::to_string(materialResource->GetUid()));

	float color[4];
	materialResource->GetColor(color[0], color[1], color[2], color[3]);

	unsigned int size = sizeof(color) + sizeof(int);
	char* fileBuffer = new char[size];
	char* pointer = fileBuffer;

	//Save color
	unsigned int bytes = sizeof(color);
	memcpy(pointer, color, bytes);
	pointer += bytes;

	//Save texture id
	bytes = sizeof(int);
	int id = materialResource->GetResourceTexture();
	memcpy(pointer, &id, bytes);
	
	
	App->fileManager->Save(filePath.c_str(), fileBuffer, size);
	App->editor->AddLog("Log: Saved material %s: ", materialResource->GetAssetPath());
	
	delete[] fileBuffer;
	fileBuffer = nullptr;
	
	return filePath;
}