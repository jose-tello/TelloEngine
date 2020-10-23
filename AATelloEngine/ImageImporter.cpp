#include "ImageImporter.h"

#include "Application.h"
#include "M_Renderer3D.h"

#pragma comment( lib, "Devil/libx86/DevIL.lib" )
#include "Devil\include\ilu.h"
#pragma comment( lib, "Devil/libx86/ILU.lib" )
#include "Devil\include\ilut.h"
#pragma comment( lib, "Devil/libx86/ILUT.lib" )


void ImageImporter::Init()
{
	ilInit();
	iluInit();
	ilutRenderer(ILUT_OPENGL);
}


void ImageImporter::Load(char* buffer, unsigned int bytes)
{
	unsigned int texId;
	ILuint imgName = 0;
	ilGenImages(1, &imgName);
	ilBindImage(imgName);

	ilLoadL(IL_TYPE_UNKNOWN, (const void*)buffer, bytes);

	texId = ilutGLBindTexImage();
	ilDeleteImage(imgName);

	App->renderer3D->AddTextureToAllMeshes(texId);
}