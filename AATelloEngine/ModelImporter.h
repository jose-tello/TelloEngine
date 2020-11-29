#ifndef __MODEL_IMPORTER_H__
#define __MODEL_IMPORTER_H__

#include <vector>
#include <string>

class R_Model;
struct ModelNode;

struct Config;

struct aiScene;
struct aiMaterial;
struct aiMesh;
struct aiNode;

struct GameObject;

namespace ModelImporter
{
	void InitDebuggerOptions();

	void Import(const char* path, R_Model*);

	void Load(R_Model*);
	//The string returned is the path to the mesh
	void Save(std::vector<ModelNode>& modelVec, int uid);

	void CleanUp();
	void LoadToScene(R_Model*);

	namespace Private
	{
		void RewriteMeta(const char* path, std::vector<int> meshes, std::vector<int> materials);

		void LinkModelResources(std::vector<ModelNode>& nodes, std::vector<int> meshes, std::vector<int> materials);
		GameObject* SearchGameObjParent(int parent, std::vector<GameObject*> vec);

		void ImportNode(aiNode* node, const aiScene* scene, int parentId, std::vector<ModelNode>& nodeVec);
		void LoadNode(ModelNode& modelNode, Config& node);
		void SaveNode(ModelNode& modelNode, Config& node);

		void InitObject(ModelNode& object, int parentId, aiNode* node);
		int ImportMesh(aiMesh*, const char* assetPath);
		int ImportMaterial(aiMaterial* material, const char* path);

	}
}
#endif // !__MODEL_IMPORTER_H__

