#ifndef __E_SCENE_H__
#define __E_SCENE_H__

#include "E_Window.h"

class E_Scene : public E_Window
{
public:
	E_Scene(bool active);
	~E_Scene();

	bool Start() override;
	bool Update() override;
	bool Draw() override;

private:
	int windowWidth;
	int windoHeight;
};

#endif // !__E_SCENE_H__

