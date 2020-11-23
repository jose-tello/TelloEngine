#ifndef __E_SCENE_H__
#define __E_SCENE_H__

#include "E_Window.h"

class C_Camera;

class W_Scene : public E_Window
{
public:
	W_Scene(bool active, C_Camera*);
	~W_Scene() override;

	bool Draw() override;

	void GetWindowMeasures(int& width, int& height) const;
	void ScreenToWorld(float& x, float& y) const;

private:
	int windowWidth;
	int windoHeight;

	unsigned int frameBuffer;
	unsigned int textureBuffer;
	unsigned int depthBuffer;

	C_Camera* camera;
};

#endif // !__E_SCENE_H__

