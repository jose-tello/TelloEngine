#include "ImageImporter.h"

#include "Application.h"
#include "M_FileManager.h"

#pragma comment( lib, "Devil/libx86/DevIL.lib" )
#include "Devil\include\ilu.h"
#pragma comment( lib, "Devil/libx86/ILU.lib" )
#include "Devil\include\ilut.h"
#pragma comment( lib, "Devil/libx86/ILUT.lib" )


void ImageImporter::Init()
{
	ilInit();
	iluInit();
	ilEnable(IL_ORIGIN_SET);
	ilOriginFunc(IL_ORIGIN_UPPER_LEFT);
	ilutRenderer(ILUT_OPENGL);
}


unsigned int ImageImporter::Load(const char* path, bool pathFromFileManager)
{
	char* buffer = nullptr;
	unsigned int bytes = 0;
	unsigned int texId;
	ILuint imgName = 0;
	std::string finalPath(path);

	if (pathFromFileManager == true)
		App->assetManager->AdaptPath(finalPath);
	
	else
	{
		std::string fileName;
		std::string fileExtension;
		App->assetManager->SplitPath(path, nullptr, &fileName, &fileExtension);
		finalPath = fileName + "." + fileExtension;
	}
	
	bytes = App->assetManager->ReadBytes(finalPath.c_str(), &buffer);

	ilGenImages(1, &imgName);
	ilBindImage(imgName);

	ilLoadL(IL_TYPE_UNKNOWN, (const void*)buffer, bytes);

	texId = ilutGLBindTexImage();
	
	ilDeleteImage(imgName);
	delete[] buffer;

	return texId;
}