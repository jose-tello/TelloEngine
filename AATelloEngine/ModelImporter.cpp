#include "ModelImporter.h"
#include "MaterialImporter.h"
#include "MeshImporter.h"

#include "Config.h"

#include "Application.h"
#include "M_Editor.h"
#include "M_FileManager.h"

#include "M_Resources.h"
#include "R_Model.h"
#include "R_Mesh.h"
#include "R_Material.h"

#include "C_Material.h"
#include "Assimp/include/material.h"

#include "MathGeoLib/src/MathGeoLib.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#pragma comment (lib, "Assimp/libx86/assimp.lib")

#include <stack>

void ModelImporter::InitDebuggerOptions()
{
	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);
}


void ModelImporter::Import(const char* path, R_Model* model)
{
	std::string filePath(path);
	App->fileManager->AdaptPath(filePath);

	char* buffer = nullptr;
	unsigned int bytes = App->fileManager->ReadBytes(filePath.c_str(), &buffer);

	std::vector<ModelNode> modelNodes;

	const aiScene* scene = aiImportFileFromMemory(buffer, bytes, aiProcessPreset_TargetRealtime_MaxQuality, nullptr);

	if (scene != nullptr && scene->HasMeshes())
	{
		std::vector<int> meshesId;
		for (int i = 0; i < scene->mNumMeshes; ++i)
			meshesId.push_back(Private::ImportMesh(scene->mMeshes[i], filePath.c_str()));
		
		std::vector<int> materialsId;
		for (int i = 0; i < scene->mNumMaterials; i++)
			materialsId.push_back(Private::ImportMaterial(scene->mMaterials[i], filePath.c_str()));

		Private::ImportNode(scene->mRootNode, scene, 0, modelNodes);
		Private::LinkModelResources(modelNodes, meshesId, materialsId);

		Save(model);

		aiReleaseImport(scene);
	}

	else
		App->editor->AddLog("[ERROR] loading scene");
	

	delete[] buffer;
	buffer = nullptr;
}


void ModelImporter::Private::LinkModelResources(std::vector<ModelNode>& nodes, std::vector<int> meshes, std::vector<int> materials)
{
	int nodesCount = nodes.size();
	for (int i = 0; i < nodesCount; i++)
	{
		if (nodes[i].meshId == -1)
			nodes[i].meshId = 0;

		else
			nodes[i].meshId = meshes[nodes[i].meshId];


		
	}

}


void ModelImporter::Private::ImportNode(aiNode* node, const aiScene* scene, int parentId, std::vector<ModelNode>& nodeVec)
{
	ModelNode obj;
	InitObject(obj, parentId, node);

	for (int i = 0; i < node->mNumMeshes; i++)
	{
		obj.meshId = node->mMeshes[i];
		obj.materialId = scene->mMeshes[obj.meshId]->mMaterialIndex;

		nodeVec.push_back(obj);

		if (node->mNumMeshes > 1) // if there is more than one mesh, create a sibiling
			InitObject(obj, parentId, node);
	}

	for (int i = 0; i < node->mNumChildren; i++)
	{
		Private::ImportNode(node->mChildren[i], scene, obj.uid, nodeVec);
	}
}


void ModelImporter::Private::InitObject(ModelNode& object, int parentId, aiNode* node)
{
	LCG randomNumber;
	object.uid = randomNumber.IntFast();

	object.parentId = parentId;

	std::string str(node->mName.C_Str());
	str = str.substr(0, str.find_first_of("$"));
	object.name = str;

	aiVector3D position;
	aiQuaternion rotation;
	aiVector3D scale;

	node->mTransformation.Decompose(scale, rotation, position);

	object.position[0] = position.x;
	object.position[1] = position.y;
	object.position[2] = position.z;

	object.rotation[0] = rotation.x;
	object.rotation[1] = rotation.y;
	object.rotation[2] = rotation.z;
	object.rotation[3] = rotation.w;

	object.scale[0] = scale.x;
	object.scale[1] = scale.y;
	object.scale[2] = scale.z;
}


int ModelImporter::Private::ImportMesh(aiMesh* mesh, const char* assetPath)
{
	return MeshImporter::Import(mesh, assetPath);
}


int ModelImporter::Private::ImportMaterial(aiMaterial* mat, const char* path)
{
	aiColor4D color;

	bool hasTextures = mat->GetTextureCount(aiTextureType::aiTextureType_DIFFUSE) > 0;
	bool hasColor = aiGetMaterialColor(mat, AI_MATKEY_COLOR_DIFFUSE, &color) == aiReturn_SUCCESS;

	if (hasTextures || hasColor)
		return MaterialImporter::Import(mat, Color(color.r, color.g, color.b, color.a), hasTextures, hasColor, path);

	return 0;
}



void ModelImporter::Load(R_Model* model)
{
	std::string filePath(MODEL_LIBRARY);
	filePath.append(std::to_string(model->GetUid()));

	char* fileBuffer;
	App->fileManager->Load(filePath.c_str(), &fileBuffer);

	Config rootNode(fileBuffer);
	ConfigArray nodeArray = rootNode.GetArray("game objects");

	int nodeCount = nodeArray.GetSize();
	for (int i = 0; i < nodeCount; i++)
	{
		Config node = nodeArray.GetNode(i);
		ModelNode modelNode;

		modelNode.uid = node.GetNum("uid");
		modelNode.parentId = node.GetNum("parent");
		modelNode.name = node.GetString("name");

		ConfigArray arr = node.GetArray("position");
		modelNode.position[0] = arr.GetNum(0);
		modelNode.position[1] = arr.GetNum(1);
		modelNode.position[2] = arr.GetNum(2);

		arr = node.GetArray("rotation");
		modelNode.rotation[0] = arr.GetNum(0);
		modelNode.rotation[1] = arr.GetNum(1);
		modelNode.rotation[2] = arr.GetNum(2);
		modelNode.rotation[3] = arr.GetNum(3);

		arr = node.GetArray("scale");
		modelNode.scale[0] = arr.GetNum(0);
		modelNode.scale[1] = arr.GetNum(1);
		modelNode.scale[2] = arr.GetNum(2);

		modelNode.meshId = node.GetNum("meshId");
		modelNode.materialId = node.GetNum("MaterialId");
	}

	delete[] fileBuffer;
	fileBuffer = nullptr;
}


void ModelImporter::Private::LoadNode(ModelNode& modelNode, Config& node)
{

}


//The string returned is the path to the model
void ModelImporter::Save(R_Model* model)
{
	std::string filePath(MODEL_LIBRARY);
	filePath.append(std::to_string(model->GetUid()));

	Config nodeRoot;
	ConfigArray nodeArray = nodeRoot.AppendArray("nodes");

	std::vector<ModelNode> modelVec;
	model->GetModelNodes(modelVec);

	int nodeCount = modelVec.size();
	for (int i = 0; i < nodeCount; i++)
	{
		Config node = nodeArray.AppendNode();
		Private::SaveNode(modelVec[i], node);
	}

	char* fileBuffer;
	unsigned int size = nodeRoot.Serialize(&fileBuffer);
	App->fileManager->Save(filePath.c_str(), fileBuffer, size);

	delete[] fileBuffer;
	fileBuffer = nullptr;
}


void ModelImporter::Private::SaveNode(ModelNode& modelNode, Config& node)
{
	node.AppendNum("uid", modelNode.uid);
	node.AppendNum("parent", modelNode.parentId);
	node.AppendString("name", modelNode.name.c_str());

	ConfigArray arr = node.AppendArray("position");
	arr.AppendNum(modelNode.position[0]);
	arr.AppendNum(modelNode.position[1]);
	arr.AppendNum(modelNode.position[2]);

	arr = node.AppendArray("rotation");
	arr.AppendNum(modelNode.rotation[0]);
	arr.AppendNum(modelNode.rotation[1]);
	arr.AppendNum(modelNode.rotation[2]);
	arr.AppendNum(modelNode.rotation[3]);

	arr = node.AppendArray("scale");
	arr.AppendNum(modelNode.scale[0]);
	arr.AppendNum(modelNode.scale[1]);
	arr.AppendNum(modelNode.scale[2]);

	node.AppendNum("meshId", modelNode.meshId);
	node.AppendNum("materialId", modelNode.materialId);
}


void ModelImporter::CleanUp()
{
	aiDetachAllLogStreams();
}