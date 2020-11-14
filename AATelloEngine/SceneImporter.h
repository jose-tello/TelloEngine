#ifndef __SCENE_IMPORTER_H__
#define __SCENE_IMPORTER_H__

#include <vector>

struct Config;
struct GameObject;
class Component;

namespace SceneImporter
{
	void Load(const char* path, std::vector<GameObject*> objVector);
	void Save(const char* sceneName, std::vector<GameObject*> objVector);

	void SaveGameObject(Config& node, GameObject* object);

	void SaveTransformComponent(Config& node, Component* component);
	void SaveMeshComponent(Config& node, Component* component);
	void SaveMaterialComponent(Config& node, Component* component);
}

#endif // !__SCENE_IMPORTER_H__
