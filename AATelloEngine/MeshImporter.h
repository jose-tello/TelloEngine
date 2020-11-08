#ifndef __MODEL_IMPORTER_H__
#define __MODEL_IMPORTER_H__

#define MAX_TEX_COORDS 8

#include <vector>

struct GameObject;
struct Mesh;
struct aiMaterial;
struct aiMesh;
struct aiNode;

namespace ModelImporter
{
	void InitDebuggerOptions();

	bool Import(char* buffer, unsigned int bytes);

	void SetObjName(GameObject* gameObject, aiNode* node);
	void InitTransformComponent(GameObject* gameObject, aiNode* node);
	void InitMeshComponent(GameObject* gameObject, aiMesh* mesh);
	void InitMaterialComponent(GameObject* gameObject, aiMaterial* material);

	void Load(Mesh* mesh, char* buffer);
	void Save(Mesh* mesh, const char* fileName);

	void CleanUp();
}
#endif // !__MODEL_IMPORTER_H__

