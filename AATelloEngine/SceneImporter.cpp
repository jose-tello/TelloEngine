#include "SceneImporter.h"
#include "M_Scene.h"

#include "Config.h"
#include "GameObject.h"

#include "Application.h"
#include "M_FileManager.h"
#include "MeshImporter.h"
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
		GameObject* parent = SearchParent(parentId, objVector);

		object = new GameObject(name, parent, uuid);
		parent->childs.push_back(object);
	}

	else	//If it does not have a parent
	{
		object = new GameObject(name, nullptr, uuid);
		objVector.push_back(object);
	}

	ConfigArray componentsArray = node.GetArray("components");
	int componentsCount = componentsArray.GetSize();

	for (int i = 0; i < componentsCount; i++)
	{
		Config componentNode = componentsArray.GetNode(i);
		int type = componentNode.GetNum("type");

		switch (COMPONENT_TYPE(type))
		{
		case COMPONENT_TYPE::TRANSFORM:
			LoadTransformComponent(componentNode, object);
			break;

		case COMPONENT_TYPE::MESH:
			object->AddComponent(LoadMeshComponent(componentNode));
			break;

		case COMPONENT_TYPE::MATERIAL:
			object->AddComponent(LoadMaterialComponent(componentNode));
			break;

		default:
			assert("Invalid component type");
			break;
		}
	}

}


void SceneImporter::LoadTransformComponent(Config& node, GameObject* object)
{
	float x, y, z, w;

	ConfigArray arr = node.GetArray("position");
	x = arr.GetNum(0);
	y = arr.GetNum(1);
	z = arr.GetNum(2);

	object->transform.SetPos(x, y, z);

	arr = node.GetArray("rotation");
	x = arr.GetNum(0);
	y = arr.GetNum(1);
	z = arr.GetNum(2);
	w = arr.GetNum(3);

	object->transform.SetRotation(w, x, y, z);

	arr = node.GetArray("scale");
	x = arr.GetNum(0);
	y = arr.GetNum(1);
	z = arr.GetNum(2);

	object->transform.SetEscale(x, y, z);
}


C_Mesh* SceneImporter::LoadMeshComponent(Config& node)
{
	C_Mesh* componentMesh = new C_Mesh();
	Mesh* mesh = new Mesh();

	const char* meshName = node.GetString("mesh");
	std::string meshPath(MESH_LIBRARY);
	meshPath.append(meshName);

	MeshImporter::Load(mesh, meshPath.c_str());

	componentMesh->SetMesh(mesh);
	return componentMesh;
}


C_Material* SceneImporter::LoadMaterialComponent(Config& node)
{
	C_Material* material = new C_Material();

	const char* materialName = node.GetString("material");
	std::string materialPath(MATERIAL_LIBRARY);
	materialPath.append(materialName);

	MaterialImporter::Load(material, materialPath.c_str());

	return material;
}


GameObject* SceneImporter::SearchParent(int uuid, std::vector<GameObject*>& objVector)
{
	int objCount = objVector.size();

	for (int i = objCount - 1; i >= 0; i--)
	{
		if (objVector[i]->GetUuid() == uuid)
			return objVector[i];
	}

	return nullptr;
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
		SaveGameObject(node, objVector[i]);
	}

	char* fileBuffer;
	unsigned int size = sceneRoot.Serialize(&fileBuffer);
	App->fileManager->Save(filePath.c_str(), fileBuffer, size);
}


void SceneImporter::SaveGameObject(Config& node, GameObject* object)
{
	node.AppendNum("uuid", object->GetUuid());

	if (object->parent != nullptr)
		node.AppendNum("parent", object->parent->GetUuid());

	else
		node.AppendNum("parent", 0);

	node.AppendString("name", object->GetName());

	ConfigArray componentsArray = node.AppendArray("components");
	Config componentNode;

	std::vector<Component*> componentsVector;
	object->GetAllComponents(componentsVector);

	int componentsCount = componentsVector.size();
	for (int i = 0; i < componentsCount; i++)
	{
		componentNode = componentsArray.AppendNode();

		switch (componentsVector[i]->GetType())
		{
		case COMPONENT_TYPE::TRANSFORM:
			componentNode.AppendNum("type", (int)COMPONENT_TYPE::TRANSFORM);
			SaveTransformComponent(componentNode, componentsVector[i]);
			break;

		case COMPONENT_TYPE::MESH:
			componentNode.AppendNum("type", (int)COMPONENT_TYPE::MESH);
			SaveMeshComponent(componentNode, componentsVector[i]);
			break;

		case COMPONENT_TYPE::MATERIAL:
			componentNode.AppendNum("type", (int)COMPONENT_TYPE::MATERIAL);
			SaveMaterialComponent(componentNode, componentsVector[i]);
			break;

		default:
			break;
		}
	}
}


void SceneImporter::SaveTransformComponent(Config& node, Component* component)
{
	float x, y, z, w;
	C_Transform* transform = (C_Transform*)component;

	ConfigArray arr = node.AppendArray("position");

	transform->GetPos(x, y, z);
	arr.AppendNum(x);
	arr.AppendNum(y);
	arr.AppendNum(z);

	arr = node.AppendArray("rotation");

	transform->GetRotation(w, x, y, z);
	arr.AppendNum(x);
	arr.AppendNum(y);
	arr.AppendNum(z);
	arr.AppendNum(w);

	arr = node.AppendArray("scale");

	transform->GetEscale(x, y, z);
	arr.AppendNum(x);
	arr.AppendNum(y);
	arr.AppendNum(z);
}


void SceneImporter::SaveMeshComponent(Config& node, Component* component)
{
	//TODO: This will be changed into onlly saving the mesh id
	C_Mesh* mesh = (C_Mesh*)component;
	node.AppendString("mesh", mesh->GetMesh()->meshPath.c_str());
}


void SceneImporter::SaveMaterialComponent(Config& node, Component* component)
{
	//TODO: This will be changed into onlly saving the material id
	C_Material* material = (C_Material*)component;
	node.AppendString("material", material->textureName.c_str());
}