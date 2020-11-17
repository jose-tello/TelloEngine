#ifndef __MESH_IMPORTER_H__
#define __MESH_IMPORTER_H__

#define MAX_TEX_COORDS 8

struct GameObject;
struct aiMesh;
struct Mesh;

#include <string>

namespace MeshImporter
{
	void Import(GameObject* gameObject, aiMesh* mesh);

	void Load(Mesh*, const char* path);
	std::string Save(Mesh*, const char* path);
}

#endif // !__MESH_IMPORTER_H__
