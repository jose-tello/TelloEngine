#include "ModelImporter.h"
#include "MaterialImporter.h"
#include "MeshImporter.h"

#include "Config.h"

#include "Application.h"
#include "M_Editor.h"

#include "GameObject.h"
#include "C_Mesh.h"
#include "Mesh.h"
#include "C_Material.h"

#include "M_Scene.h"
#include "M_FileManager.h"

#include "MathGeoLib/include/MathGeoLib.h"

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


void ModelImporter::Import(const char* path)
{
	std::string filePath(path);
	App->fileManager->AdaptPath(filePath);

	char* buffer = nullptr;
	unsigned int bytes = App->fileManager->ReadBytes(filePath.c_str(), &buffer);

	std::string fileName;
	App->fileManager->SplitPath(filePath.c_str(), nullptr, &fileName, nullptr);

	std::stack<GameObject*> objStack;
	std::vector<GameObject*> objToSave;
	GameObject* root = new GameObject(nullptr);
	GameObject* obj = nullptr;

	std::stack<aiNode*> nodeStack;
	aiNode* node = nullptr;

	const aiScene* scene = aiImportFileFromMemory(buffer, bytes, aiProcessPreset_TargetRealtime_MaxQuality, nullptr);

	if (scene != nullptr && scene->HasMeshes())
	{
		nodeStack.push(scene->mRootNode);
		objStack.push(root);

		while (nodeStack.empty() == false)
		{
			node = nodeStack.top();
			nodeStack.pop();

			obj = objStack.top();
			objStack.pop();
			objToSave.push_back(obj);

			Private::SetObjName(obj, node);
			Private::InitTransformComponent(obj, node);

			for (int i = 0; i < node->mNumMeshes; i++)
			{
				unsigned int meshIterator = node->mMeshes[i];

				aiMesh* mesh = scene->mMeshes[meshIterator];
				MeshImporter::Import(obj, mesh);
				
				aiMaterial* material = scene->mMaterials[scene->mMeshes[meshIterator]->mMaterialIndex];
				Private::InitMaterialComponent(obj, material, obj->GetName());

				if (node->mNumMeshes > 1) // if there is more than one mesh, create a sibiling
				{
					GameObject* it = new GameObject(obj->parent);
					obj->parent->childs.push_back(it);
					obj = it;
					Private::SetObjName(obj, node);
					Private::InitTransformComponent(obj, node);

					objToSave.push_back(obj);
				}
			}

			for (int i = 0; i < node->mNumChildren; i++)
			{
				nodeStack.push(node->mChildren[i]);

				obj->childs.push_back(new GameObject(obj));
				objStack.push(obj->childs[i]);
			}
		}

		Save(objToSave, fileName.c_str());
		App->scene->AddGameObject(root);

		aiReleaseImport(scene);
	}

	else
		App->editor->AddLog("[ERROR] loading scene");
	

	delete[] buffer;
	buffer = nullptr;
}


void ModelImporter::Private::SetObjName(GameObject* object, aiNode* node)
{
	std::string str(node->mName.C_Str());

	str = str.substr(0, str.find_first_of("$"));
	object->SetName(str.c_str());
}


void ModelImporter::Private::InitTransformComponent(GameObject* object, aiNode* node)
{
	aiVector3D position;
	aiQuaternion rotation;
	aiVector3D scale;

	node->mTransformation.Decompose(scale, rotation, position);

	Quat quat(rotation.x, rotation.y, rotation.z, rotation.w);
	float3 pos = {position.x, position.y, position.z};
	float3 scl = { scale.x, scale.y, scale.z };

	float4x4 transform = float4x4::FromTRS(pos, quat, scl);

	object->transform.AddTransform(transform);
}


void ModelImporter::Private::InitMaterialComponent(GameObject* gameObject, aiMaterial* mat, const char* nodeName)
{
	aiColor4D color;

	bool hasTextures = mat->GetTextureCount(aiTextureType::aiTextureType_DIFFUSE) > 0;
	bool hasColor = aiGetMaterialColor(mat, AI_MATKEY_COLOR_DIFFUSE, &color) == aiReturn_SUCCESS;

	if (hasTextures || hasColor)
	{
		C_Material* material = new C_Material();
		MaterialImporter::Import(mat, material, Color(color.r, color.g, color.b, color.a), hasTextures, hasColor, nodeName);

		gameObject->AddComponent(material);
	}	
}



void ModelImporter::Load(GameObject* root, const char* path)
{
	std::string filePath(MODEL_LIBRARY);
	filePath.append(path);

	char* fileBuffer;
	App->fileManager->Load(filePath.c_str(), &fileBuffer);

	Config rootNode(fileBuffer);
	ConfigArray gameObjects = rootNode.GetArray("game objects");
	int objCount = gameObjects.GetSize();

	int uuid = rootNode.GetNum("uuid");
	std::string name = rootNode.GetString("name");

	GameObject* object = new GameObject(name, nullptr, uuid); //Root
	App->scene->AddGameObject(object);

	for (int i = 1; i < objCount; i++)
	{
		Config node = gameObjects.GetNode(i);
		
		int uuid = node.GetNum("uuid");
		int parentId = node.GetNum("parent");
		std::string name = node.GetString("name");

		GameObject* parent = App->scene->GetGameObject(uuid);

		object = new GameObject(name, parent, uuid);
		parent->childs.push_back(object);
	}

	delete[] fileBuffer;
	fileBuffer = nullptr;
}


//The string returned is the path to the model
std::string ModelImporter::Save(std::vector<GameObject*>& allGo, const char* fileName)
{
	std::string filePath(MODEL_LIBRARY);
	filePath.append(fileName);

	Config sceneRoot;
	
	sceneRoot.AppendString("Model name", fileName);

	ConfigArray gameObjects = sceneRoot.AppendArray("game objects");

	int objCount = allGo.size();
	for (int i = 0; i < objCount; i++)
	{
		Config node = gameObjects.AppendNode();
		allGo[i]->Save(node);
	}

	char* fileBuffer;
	unsigned int size = sceneRoot.Serialize(&fileBuffer);
	App->fileManager->Save(filePath.c_str(), fileBuffer, size);

	delete[] fileBuffer;
	fileBuffer = nullptr;

	return filePath;
}


void ModelImporter::CleanUp()
{
	aiDetachAllLogStreams();
}