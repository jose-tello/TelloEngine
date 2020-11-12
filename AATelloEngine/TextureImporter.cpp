#include "TextureImporter.h"

#include "Application.h"
#include "M_FileManager.h"
#include "M_Editor.h"

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


unsigned int TextureImporter::Import(const char* path, bool pathFromFileManager)
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
	Save(finalPath.c_str());

	ilDeleteImage(imgName);
	delete[] buffer;
	buffer = nullptr;

	return texId;
}


unsigned int TextureImporter::Load(const char* path)
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


void TextureImporter::Save(const char* name)
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
			fileName.append(name);
			App->fileManager->Save(fileName.c_str(), fileBuffer, size);
		}
		else
			App->editor->AddLog("[ERROR]: Error while saving texture");

		delete[] data;
		data = nullptr;
	}
}