#include "ImageImporter.h"

#include "Application.h"
#include "M_Editor.h"
#include "E_Inspector.h"

#include "GameObject.h"
#include "C_Material.h"

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


void ImageImporter::Load(char* buffer, unsigned int bytes)
{
	unsigned int texId;
	ILuint imgName = 0;

	E_Inspector* inspector = (E_Inspector*)App->editor->GetWindow(E_WINDOW_TYPE::INSPECTOR);
	GameObject* object = inspector->GetFocusedGameObject();

	if (object != nullptr)
	{
		ilGenImages(1, &imgName);
		ilBindImage(imgName);

		ilLoadL(IL_TYPE_UNKNOWN, (const void*)buffer, bytes);

		texId = ilutGLBindTexImage();
		ilDeleteImage(imgName);

		C_Material* material = new C_Material(object);
		material->SetTexture(texId);

		object->AddComponent(material);
	}
	else
		App->editor->AddLog("WARNING: Dont have a game object selected");
	
}