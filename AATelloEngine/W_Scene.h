#ifndef __E_SCENE_H__
#define __E_SCENE_H__

#include "E_Window.h"

class W_Scene : public E_Window
{
public:
	W_Scene(bool active);
	~W_Scene() override;

	bool Draw() override;

private:
	int windowWidth;
	int windoHeight;
};

#endif // !__E_SCENE_H__

