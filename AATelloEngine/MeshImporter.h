#ifndef __MODEL_IMPORTER_H__
#define __MODEL_IMPORTER_H__

#define MAX_TEX_COORDS 8

#include <vector>

struct GameObject;
struct aiMaterial;
struct aiNode; 

namespace ModelImporter
{
	void InitDebuggerOptions();

	bool Load(char* buffer, unsigned int bytes);

	void SetObjName(GameObject*, aiNode*);
	void InitTransformComponent(GameObject*, aiNode*);
	void InitMeshComponent(GameObject*, std::vector<float>& vertices, std::vector<float>& normals, std::vector<float>& texCoords, std::vector<unsigned int>& indices);
	void InitMaterialComponent(GameObject*, aiMaterial* mat);

	void CleanUp();
}
#endif // !__MODEL_IMPORTER_H__

