#ifndef __MESH_IMPORTER_H__
#define __MESH_IMPORTER_H__

#define MAX_TEX_COORDS 8

struct aiMesh;
class R_Mesh;

#include <string>

namespace MeshImporter
{
	int Import(aiMesh* gameObject, const char* assetPath);

	void Load(R_Mesh*);
	void Save(R_Mesh*);
}

#endif // !__MESH_IMPORTER_H__
