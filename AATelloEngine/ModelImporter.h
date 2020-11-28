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

namespace ModelImporter
{
	void InitDebuggerOptions();

	void Import(const char* path, R_Model*);
	

	void Load(R_Model*);
	//The string returned is the path to the mesh
	void Save(R_Model*);

	void CleanUp(); 


	namespace Private
	{
		void ImportNode(aiNode* node, const aiScene* scene, int parentId, std::vector<ModelNode>& nodeVec);
		void LoadNode(ModelNode& modelNode, Config& node);
		void SaveNode(ModelNode& modelNode, Config& node);

		void InitObject(ModelNode& object, int parentId, aiNode* node);
		void ImportMesh(ModelNode& object, aiMesh*);
		void ImportMaterial(ModelNode& gameObject, aiMaterial* material);

	}
}
#endif // !__MODEL_IMPORTER_H__

