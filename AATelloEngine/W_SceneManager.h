#ifndef __E_SCENE_MANAGER__
#define __E_SCENE_MANAGER__

#include "E_Window.h"

#include <string>
#include <vector>

#define MAX_SCENE_NAME_LENGTH 100

class W_SceneManager : public E_Window
{
public:
	W_SceneManager(bool open = true);
	~W_SceneManager();

	bool Draw() override;

private:
	void GetSceneNames(const char* directory);

private:
	std::vector<std::string> sceneNames;

};
#endif // !__E_SCENE_MANAGER__

