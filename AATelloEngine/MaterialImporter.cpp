#include "MaterialImporter.h"

#include "Application.h"
#include "M_FileManager.h"

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
	if (hasTexture)
	{
		aiString texPath;
		material->GetTexture(aiTextureType::aiTextureType_DIFFUSE, 0, &texPath);

		std::string fileName;
		std::string fileExtension;
		App->fileManager->SplitPath(texPath.C_Str(), nullptr, &fileName, &fileExtension);
		fileName = fileName + "." + fileExtension;

		materialComponent->SetTexture(ImportTexture(fileName.c_str(), false));
		materialComponent->texturePath = texPath.C_Str();
	}

	if (hasColor)
		materialComponent->SetColor(color);
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
	
	ilDeleteImage(imgName);
	delete[] buffer;

	return texId;
}


void MaterialImporter::Load(const char* path)
{

}


//The string returned is the path to the mesh
std::string MaterialImporter::Save(const char* fileName)
{
	return std::string("a");
}