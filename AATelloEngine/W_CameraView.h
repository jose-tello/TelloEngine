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
	void ScreenToWorld(float& x, float& y) const;

protected:
	int windowWidth;
	int windoHeight;

	unsigned int frameBuffer;
	unsigned int textureBuffer;
	unsigned int depthBuffer;

	C_Camera* camera;
};

#endif // !__E_CAMERA_VIEW_H__

