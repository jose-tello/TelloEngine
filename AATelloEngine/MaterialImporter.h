#ifndef __IMAGE_IMPORTER_H__
#define __IMAGE_IMPORTER_H__

struct aiMaterial;
struct Color;
class R_Material;

#include <string>

namespace MaterialImporter
{
	int Import(aiMaterial* material, Color& col, bool hasTexture, bool hasColor, const char* assetPath);
	
	void Load(R_Material*);
	
	//The string returned is the path to the mesh
	void Save(R_Material*);
}

#endif // !__IMAGE_IMPORTER_H__
