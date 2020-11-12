#include "MaterialImporter.h"

#include "Application.h"
#include "M_FileManager.h"
#include "M_Editor.h"

#include "C_Material.h"

#include "Assimp/include/material.h"

#pragma comment( lib, "Devil/libx86/DevIL.lib" )
#include "Devil\include\ilu.h"
#pragma comment( lib, "Devil/libx86/ILU.lib" )
#include "Devil\include\ilut.h"
#pragma comment( lib, "Devil/libx86/ILUT.lib" )


void MaterialImporter::Init()
{
	ilInit();
	iluInit();
	ilEnable(IL_ORIGIN_SET);
	ilOriginFunc(IL_ORIGIN_UPPER_LEFT);
	ilutRenderer(ILUT_OPENGL);
}


void MaterialImporter::Import(aiMaterial* material, C_Material* materialComponent, Color& color, bool hasTexture, bool hasColor)
{
	std::string materialName("noName");
	if (hasTexture)
	{
		aiString texPath;
		material->GetTexture(aiTextureType::aiTextureType_DIFFUSE, 0, &texPath);

		std::string fileName;
		std::string fileExtension;
		App->fileManager->SplitPath(texPath.C_Str(), nullptr, &materialName, &fileExtension);
		fileName = materialName + "." + fileExtension;

		materialComponent->SetTexture(ImportTexture(fileName.c_str(), false));
		materialComponent->texturePath = texPath.C_Str();
		materialComponent->textureName = materialName;
	}

	if (hasColor)
		materialComponent->SetColor(color);

	std::string materialPath = Save(materialComponent, materialName.c_str());
	Load(materialComponent, materialPath.c_str());
}


unsigned int MaterialImporter::ImportTexture(const char* path, bool pathFromFileManager)
{
	char* buffer = nullptr;
	unsigned int bytes = 0;
	unsigned int texId = 0;
	ILuint imgName = 0;
	std::string finalPath(path);

	if (pathFromFileManager == true)
		App->fileManager->AdaptPath(finalPath);

	bytes = App->fileManager->ReadBytes(finalPath.c_str(), &buffer);

	ilGenImages(1, &imgName);
	ilBindImage(imgName);

	ilLoadL(IL_TYPE_UNKNOWN, (const void*)buffer, bytes);

	texId = ilutGLBindTexImage();

	App->fileManager->SplitPath(path, nullptr, &finalPath, nullptr);
	SaveTexture(finalPath.c_str());

	ilDeleteImage(imgName);
	delete[] buffer;
	buffer = nullptr;

	return texId;
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

		std::string filePath(TEXTURE_LIBRARY);
		filePath.append("/");
		filePath.append(pointer);	//Name of the file
		material->SetTexture(LoadTexture(filePath.c_str()));
	}

	delete[] fileBuffer;
	fileBuffer = nullptr;

	App->editor->AddLog("Log: Loaded a material");
}


unsigned int MaterialImporter::LoadTexture(const char* path)
{
	char* fileBuffer = nullptr;
	unsigned int bytes = 0;
	unsigned int texId = 0;
	ILuint imgName = 0;

	bytes = App->fileManager->ReadBytes(path, &fileBuffer);

	ilGenImages(1, &imgName);
	ilBindImage(imgName);

	ilLoadL(IL_TYPE_UNKNOWN, fileBuffer, bytes);

	texId = ilutGLBindTexImage();

	ilDeleteImage(imgName);
	delete[] fileBuffer;
	fileBuffer = nullptr;

	return texId;
}


//The string returned is the path to the mesh
std::string MaterialImporter::Save(C_Material* materialComponent, const char* fileName)
{
	std::string filePath(MATERIAL_LIBRARY);
	filePath.append("/");
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
	bytes = sizeof(char) * materialComponent->textureName.length();
	const char* texName = materialComponent->textureName.c_str();
	memcpy(pointer, texName, bytes); 

	App->fileManager->Save(filePath.c_str(), fileBuffer, size);
	App->editor->AddLog("Log: Saved a material");
	
	delete[] fileBuffer;
	fileBuffer = nullptr;
	
	return filePath;
}


void MaterialImporter::SaveTexture(const char* name)
{
	unsigned int size;
	ILubyte* data;
	char* fileBuffer = nullptr;
	
	ilSetInteger(IL_DXTC_FORMAT, IL_DXT5);
	size = ilSaveL(IL_DDS, nullptr, 0);

	if (size > 0)
	{
		data = new ILubyte[size];
		if (ilSaveL(IL_DDS, data, size) > 0)
		{
			std::string fileName;
			
			fileBuffer = (char*)data;

			fileName.append(TEXTURE_LIBRARY);
			fileName.append("/");
			fileName.append(name);
			App->fileManager->Save(fileName.c_str(), fileBuffer, size);

			App->editor->AddLog("Log: Saced a texture");
		}
		else
			App->editor->AddLog("[ERROR]: Error while saving texture");

		delete[] data;
		data = nullptr;
	}
}