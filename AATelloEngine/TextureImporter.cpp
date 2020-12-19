#include "TextureImporter.h"

#include "Application.h"
#include "M_FileManager.h"
#include "M_Editor.h"

#include "R_Texture.h"

#pragma comment( lib, "Devil/libx86/DevIL.lib" )
#include "Devil\include\ilu.h"
#pragma comment( lib, "Devil/libx86/ILU.lib" )
#include "Devil\include\ilut.h"
#pragma comment( lib, "Devil/libx86/ILUT.lib" )

#include <string>

void TextureImporter::Init()
{
	ilInit();
	iluInit();
	ilEnable(IL_ORIGIN_SET);
	ilOriginFunc(IL_ORIGIN_UPPER_LEFT);
	ilutRenderer(ILUT_OPENGL);
}


//TODO: need to check its working correctlly
void TextureImporter::Import(const char* path, R_Texture* resource)
{
	char* buffer = nullptr;
	unsigned int bytes = 0;
	unsigned int texId = 0;
	ILuint imgName = 0;
	std::string finalPath(path);

	bytes = App->fileManager->ReadBytes(finalPath.c_str(), &buffer);

	ilGenImages(1, &imgName);
	ilBindImage(imgName);

	ilLoadL(IL_TYPE_UNKNOWN, (const void*)buffer, bytes);

	texId = ilutGLBindTexImage();

	Save(resource);

	ilDeleteImage(imgName);
	delete[] buffer;
	buffer = nullptr;
}


void TextureImporter::Load(R_Texture* texture)
{
	char* fileBuffer = nullptr;
	unsigned int bytes = 0;
	unsigned int texId = 0;
	ILuint imgName = 0;

	std::string path(TEXTURE_LIBRARY);
	path.append(std::to_string(texture->GetUid()));

	bytes = App->fileManager->ReadBytes(path.c_str(), &fileBuffer);

	ilGenImages(1, &imgName);
	ilBindImage(imgName);

	ilLoadL(IL_TYPE_UNKNOWN, fileBuffer, bytes);

	texture->SetTextureId(ilutGLBindTexImage());

	ilDeleteImage(imgName);
	delete[] fileBuffer;
	fileBuffer = nullptr;
}


void TextureImporter::Save(R_Texture* texture)
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
			fileBuffer = (char*)data;

			std::string fileName(TEXTURE_LIBRARY);
			fileName.append(std::to_string(texture->GetUid()));
			App->fileManager->Save(fileName.c_str(), fileBuffer, size);
		}
		else
			App->editor->AddLog("[ERROR]: Error while saving texture");

		delete[] data;
		data = nullptr;
	}
}