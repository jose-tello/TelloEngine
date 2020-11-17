#ifndef __IMAGE_IMPORTER_H__
#define __IMAGE_IMPORTER_H__

struct aiMaterial;
struct Color;
class C_Material;

#include <string>

namespace MaterialImporter
{
	void Import(aiMaterial* material, C_Material* materialComponent, Color& col, bool hasTexture, bool hasColor, const char* nodeName);
	
	void Load(C_Material*, const char* path);
	
	//The string returned is the path to the mesh
	std::string Save(C_Material* materialComponent, const char* fileName);
}

#endif // !__IMAGE_IMPORTER_H__
