#ifndef __IMAGE_IMPORTER_H__
#define __IMAGE_IMPORTER_H__

#pragma comment( lib, "Devil/libx86/DevIL.lib" )
#include "Devil\include\ilu.h"
#pragma comment( lib, "Devil/libx86/ILU.lib" )
#include "Devil\include\ilut.h"
#pragma comment( lib, "Devil/libx86/ILUT.lib" )

#include <string>

namespace ImageImporter
{
	void Init()
	{
		ilInit();
		iluInit();
		ilutRenderer(ILUT_OPENGL);
	}

	unsigned int Load(std::string path)
	{
		ILuint imgName;
		ilGenImages(1, &imgName);
		ilBindImage(imgName);

		ilLoadImage(path.c_str());

		return ilutGLBindTexImage();
	}
}

#endif // !__IMAGE_IMPORTER_H__
