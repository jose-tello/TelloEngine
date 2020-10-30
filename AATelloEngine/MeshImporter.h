#ifndef __MODEL_IMPORTER_H__
#define __MODEL_IMPORTER_H__

#define MAX_TEX_COORDS 8

#include <vector>

struct GameObject;
struct aiMaterial;
struct aiMesh;
struct aiNode; 

namespace ModelImporter
{
	void InitDebuggerOptions();

	bool Load(char* buffer, unsigned int bytes);

	void SetObjName(GameObject* gameObject, aiNode* node);
	void InitTransformComponent(GameObject* gameObject, aiNode* node);
	void InitMeshComponent(GameObject* gameObject, aiMesh* mesh);
	void InitMaterialComponent(GameObject* gameObject, aiMaterial* material);

	void CleanUp();
}
#endif // !__MODEL_IMPORTER_H__

