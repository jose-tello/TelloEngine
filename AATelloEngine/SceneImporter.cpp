#include "SceneImporter.h"
#include "M_Scene.h"

#include "Config.h"
#include "GameObject.h"

#include "Application.h"
#include "M_FileManager.h"

#include "Component.h"
#include "C_Transform.h"
#include "C_Mesh.h"
#include "C_Material.h"

#include "Mesh.h"

void SceneImporter::Load(const char* path, std::vector<GameObject*> objVector)
{

}


void SceneImporter::Save(const char* sceneName, std::vector<GameObject*> objVector)
{
	Config sceneRoot;
	Config node;
	sceneRoot.AppendString("Scene name", sceneName);

	ConfigArray gameObjects = sceneRoot.AppendArray("Game objects");

	int objCount = objVector.size();
	for (int i = 0; i < objCount; i++)
	{
		node = gameObjects.AppendNode();
		SaveGameObject(node, objVector[i]);
	}

	char* buffer;
	unsigned int size = sceneRoot.Serialize(&buffer);
	App->fileManager->Save(sceneName, &buffer, size);
	
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

	ConfigArray array = node.AppendArray("position");

	transform->GetPos(x, y, z);
	array.AppendNum(x);
	array.AppendNum(y);
	array.AppendNum(z);

	array = node.AppendArray("rotation");
	
	transform->GetRotation(w, x, y, z);
	array.AppendNum(x);
	array.AppendNum(y);
	array.AppendNum(z);
	array.AppendNum(w);

	array = node.AppendArray("scale");

	transform->GetEscale(x, y, z);
	array.AppendNum(x);
	array.AppendNum(y);
	array.AppendNum(z);
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