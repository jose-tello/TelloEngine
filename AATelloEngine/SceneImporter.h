#ifndef __SCENE_IMPORTER_H__
#define __SCENE_IMPORTER_H__

#include <vector>

struct Config;
struct GameObject;

class Component;
class C_Transform;
class C_Mesh;
class C_Material;

//TODO: add private namespace
namespace SceneImporter
{
	void Load(const char* path, std::vector<GameObject*>& objVector);

	void LoadGameObject(Config&, std::vector<GameObject*>& objVector);
	void LoadTransformComponent(Config& node, GameObject*);
	C_Mesh* LoadMeshComponent(Config& node);
	C_Material* LoadMaterialComponent(Config& node);

	GameObject* SearchParent(int uuid, std::vector<GameObject*>& objVector);


	void Save(const char* sceneName, std::vector<GameObject*> objVector);

	void SaveGameObject(Config& node, GameObject* object);

	void SaveTransformComponent(Config& node, Component* component);
	void SaveMeshComponent(Config& node, Component* component);
	void SaveMaterialComponent(Config& node, Component* component);
}

#endif // !__SCENE_IMPORTER_H__
