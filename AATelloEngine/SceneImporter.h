#ifndef __SCENE_IMPORTER_H__
#define __SCENE_IMPORTER_H__

#include <vector>

struct Config;
struct GameObject;

namespace SceneImporter
{
	void Load(const char* path, std::vector<GameObject*>& objVector);

	void Save(const char* sceneName, std::vector<GameObject*> objVector);

	namespace Private
	{
		void LoadGameObject(Config&, std::vector<GameObject*>& objVector);
	}
}

#endif // !__SCENE_IMPORTER_H__
