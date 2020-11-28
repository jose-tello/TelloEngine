#include "R_Material.h"
#include "MaterialImporter.h"

#include "Application.h"
#include "M_Resources.h"

#include "R_Texture.h"

#include "Glew/include/glew.h"
#pragma comment(lib,"Glew/libx86/glew32.lib")

#include <gl/GL.h>

R_Material::R_Material(int uid, const char* assetPath, RESOURCE_TYPE type) : Resource(uid, assetPath, type),
	color(1.f, 1.f, 1.f),
	resourceTexture(0)
{
}


R_Material::~R_Material()
{
	
}


void R_Material::Load()
{
	MaterialImporter::Load(this);
}


void R_Material::UnLoad()
{
	resourceTexture = 0;
}


void R_Material::GetColor(float& r, float& g, float& b, float& a) const
{
	r = color.r;
	g = color.g;
	b = color.b;
	a = color.a;
}


void R_Material::SetColor(Color& col)
{
	color = col;
}


int R_Material::GetResourceTexture() const
{
	return resourceTexture;
}


void R_Material::SetResourceTexture(int resource)
{
	resourceTexture = resource;
}