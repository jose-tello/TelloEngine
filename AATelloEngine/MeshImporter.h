#ifndef __MODEL_IMPORTER_H__
#define __MODEL_IMPORTER_H__

#define MAX_TEX_COORDS 8

#include <vector>
#include <string>

struct GameObject;
struct Mesh;
struct aiMaterial;
struct aiMesh;
struct aiNode;

namespace MeshImporter
{
	void InitDebuggerOptions();

	void Import(const char* path);

	void SetObjName(GameObject* gameObject, aiNode* node);
	void InitTransformComponent(GameObject* gameObject, aiNode* node);
	void InitMeshComponent(GameObject* gameObject, aiMesh* mesh);
	void InitMaterialComponent(GameObject* gameObject, aiMaterial* material);

	void Load(Mesh* mesh, const char* path);
	//The string returned is the path to the mesh
	std::string Save(Mesh* mesh, const char* fileName);

	void CleanUp();
}
#endif // !__MODEL_IMPORTER_H__

