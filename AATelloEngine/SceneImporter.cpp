#include "SceneImporter.h"
#include "M_Scene.h"

#include "Config.h"
#include "GameObject.h"

#include "Application.h"
#include "M_FileManager.h"
#include "ModelImporter.h"
#include "MaterialImporter.h"

#include "Component.h"
#include "C_Transform.h"
#include "C_Mesh.h"
#include "C_Material.h"

#include "Mesh.h"

void SceneImporter::Load(const char* path, std::vector<GameObject*>& objVector)
{
	std::string filePath(SCENE_LIBRARY);
	filePath.append(path);

	char* fileBuffer;
	App->fileManager->Load(filePath.c_str(), &fileBuffer);

	Config node(fileBuffer);
	ConfigArray gameObjects = node.GetArray("game objects");
	int objCount = gameObjects.GetSize();

	for (int i = 0; i < objCount; i++)
	{
		node = gameObjects.GetNode(i);
		LoadGameObject(node, objVector);
	}
}


void SceneImporter::LoadGameObject(Config& node, std::vector<GameObject*>& objVector)
{
	int uuid = node.GetNum("uuid");
	int parentId = node.GetNum("parent");
	std::string name = node.GetString("name");

	GameObject* object;

	if (parentId != 0)
	{
		GameObject* parent = App->scene->GetGameObject(parentId);

		object = new GameObject(name, parent, uuid);
		parent->childs.push_back(object);
	}

	else	//If it does not have a parent
	{
		object = new GameObject(name, nullptr, uuid);
		objVector.push_back(object);
	}

	object->Load(node);
}


void SceneImporter::Save(const char* sceneName, std::vector<GameObject*> objVector)
{
	std::string filePath(SCENE_LIBRARY);
	filePath.append(sceneName);

	Config sceneRoot;
	Config node;
	sceneRoot.AppendString("Scene name", sceneName);

	ConfigArray gameObjects = sceneRoot.AppendArray("game objects");

	int objCount = objVector.size();
	for (int i = 0; i < objCount; i++)
	{
		node = gameObjects.AppendNode();
		objVector[i]->Save(node);
	}

	char* fileBuffer;
	unsigned int size = sceneRoot.Serialize(&fileBuffer);
	App->fileManager->Save(filePath.c_str(), fileBuffer, size);
}