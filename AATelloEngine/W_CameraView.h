#ifndef __E_CAMERA_VIEW_H__
#define __E_CAMERA_VIEW_H__

#include "E_Window.h"

class C_Camera;

class W_CameraView : public E_Window
{
public:
	W_CameraView(bool active, C_Camera*, E_WINDOW_TYPE type = E_WINDOW_TYPE::CAMERA_VIEW);
	virtual ~W_CameraView() override;

	bool Draw() override; 
	
	void GetWindowMeasures(int& width, int& height) const;
	void GetWindowPos(float& x, float& y) const;
	void GetMousePosInWindow(float& mouseX, float& mouseY) const;

protected:
	void OnResize(int x, int y);

protected:
	int windowWidth;
	int windowHeight;

	int windowPosX;
	int windowPosY;

	int mousePosX;
	int mousePosY;

	unsigned int frameBuffer;
	unsigned int textureBuffer;
	unsigned int depthBuffer;

	C_Camera* camera = nullptr;

private:
	bool started = false;
};

#endif // !__E_CAMERA_VIEW_H__

