#ifndef __E_GAME_OBJECTS__
#define __E_GAME_OBJECTS__

#include "E_Window.h"

#include <vector>

struct GameObject;

class W_ObjectHierarchy : public E_Window
{
public:
	W_ObjectHierarchy(bool open = true);
	~W_ObjectHierarchy();

	bool Draw() override;

private:
	void DrawChildren(std::vector<GameObject*>&);
	void HandleDragAndDrop(GameObject* currGameObject);
	void ReparentGameObjects(GameObject* father, GameObject* child);

};
#endif // !__E_GAME_OBJECTS__

