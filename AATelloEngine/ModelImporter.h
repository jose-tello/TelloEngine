#ifndef __MODEL_IMPORTER_H__
#define __MODEL_IMPORTER_H__

#include <vector>
#include <string>

struct GameObject;
struct aiMaterial;
struct aiMesh;
struct aiNode;

namespace ModelImporter
{
	void InitDebuggerOptions();

	void Import(const char* path);

	void Load(GameObject* mesh, const char* path);
	//The string returned is the path to the mesh
	std::string Save(std::vector<GameObject*>& allGo, const char* fileName);

	void CleanUp(); 


	namespace Private
	{
		void SetObjName(GameObject* gameObject, aiNode* node);
		void InitTransformComponent(GameObject* gameObject, aiNode* node);
		void InitMaterialComponent(GameObject* gameObject, aiMaterial* material, const char* nodeName);
	}
}
#endif // !__MODEL_IMPORTER_H__

