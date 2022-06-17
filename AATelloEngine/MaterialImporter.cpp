#include "MaterialImporter.h"

#include "Application.h"
#include "M_FileManager.h"
#include "M_Editor.h"
#include "M_Resources.h"

#include "R_Material.h"

#include "Assimp/include/material.h"
#include "MathGeoLib/src/Algorithm/Random/LCG.h"

int MaterialImporter::Import(aiMaterial* material, Color& color, bool hasTexture, bool hasColor, const char* assetPath, int uid)
{
	int textureId = 0;

	R_Material* materialResource = nullptr;
	
	if (uid == 0)
	{
		LCG random;
		materialResource = new R_Material(random.IntFast(), assetPath, RESOURCE_TYPE::MATERIAL);
	}
	else
	{
		materialResource = new R_Material(uid, assetPath, RESOURCE_TYPE::MATERIAL);
	}
	
	if (hasTexture)
	{
		aiString texPath;
		material->GetTexture(aiTextureType::aiTextureType_DIFFUSE, 0, &texPath);

		std::string fileName;
		std::string fileExtension;
		App->fileManager->SplitPath(texPath.C_Str(), nullptr, &fileName, &fileExtension);
		fileName = "/Assets/" + fileName + "." + fileExtension;
		textureId = App->resourceManager->SearchMetaFile(fileName.c_str());

		if (textureId == 0)
			textureId = App->resourceManager->CreateResource(fileName.c_str(), false);
	}

	Save(color, textureId, materialResource->GetUid());

	App->resourceManager->PushResource(materialResource, materialResource->GetUid());
	return materialResource->GetUid();
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

		int texId = 0;
		bytes = sizeof(int);
		memcpy(&texId, pointer, bytes);
		pointer += bytes;

		int shaderId = 0;
		bytes = sizeof(int);
		memcpy(&shaderId, pointer, bytes);

		material->SetColor(Color(color[0], color[1], color[2], color[3]));
		material->SetResourceTexture(texId);
		material->SetResourceShader(shaderId);
	}

	delete[] fileBuffer;
	fileBuffer = nullptr;
	pointer = nullptr;

	App->editor->AddLog("Log: Loaded material from: %s", material->GetAssetPath().c_str());
}


//TODO: import the shader correctlly, for now saving allways the default shader
void MaterialImporter::Save(Color& col, int textureId, int uid)
{
	std::string filePath(MATERIAL_LIBRARY);
	filePath.append(std::to_string(uid));

	float color[4] = { col.r, col.g, col.b, col.a };

	unsigned int size = sizeof(color) + sizeof(int) + sizeof(int);
	char* fileBuffer = new char[size];
	char* pointer = fileBuffer;

	//Save color
	unsigned int bytes = sizeof(color);
	memcpy(pointer, color, bytes);
	pointer += bytes;

	//Save texture id
	bytes = sizeof(int);
	memcpy(pointer, &textureId, bytes);
	pointer += bytes;

	//Save shader id
	bytes = sizeof(int);
	int id = 0;
	memcpy(pointer, &id, bytes);
	
	App->fileManager->Save(filePath.c_str(), fileBuffer, size);
	
	delete[] fileBuffer;
	fileBuffer = nullptr;
}