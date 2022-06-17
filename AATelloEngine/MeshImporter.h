#ifndef __MESH_IMPORTER_H__
#define __MESH_IMPORTER_H__

#define MAX_TEX_COORDS 8

struct aiMesh;
class R_Mesh;

#include <vector>

namespace MeshImporter
{
	int Import(aiMesh* gameObject, const char* assetPath, int uid = 0);

	void Load(R_Mesh*);
	void Save(std::vector<float>&, std::vector<float>&, std::vector<float>&, std::vector<unsigned int>&, int uid);
}

#endif // !__MESH_IMPORTER_H__
