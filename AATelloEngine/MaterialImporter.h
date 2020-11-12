#ifndef __IMAGE_IMPORTER_H__
#define __IMAGE_IMPORTER_H__

struct aiMaterial;
struct Color;
class C_Material;

#include <string>

namespace MaterialImporter
{
	void Init();

	void Import(aiMaterial* material, C_Material* materialComponent, Color& col, bool hasTexture, bool hasColor);
	unsigned int ImportTexture(const char* path, bool pathFromFileManager = true);

	void Load(C_Material*, const char* path);
	unsigned int LoadTexture(const char* path);
	//The string returned is the path to the mesh
	std::string Save(C_Material* materialComponent, const char* fileName);
	void SaveTexture(const char* );
}

#endif // !__IMAGE_IMPORTER_H__
