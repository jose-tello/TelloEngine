#ifndef __W_SCENE_H__
#define	__W_SCENE_H__

#include "W_CameraView.h"

//Window of the scene camera
class W_Scene : public W_CameraView 
{
public:
	W_Scene(bool active, C_Camera*);
	~W_Scene() override;

	bool Draw() override;

	bool IsWindowHovered() const;

private:
	void HandleInput();
	void HandleGizmo();

private:
	int gizmoOperation;
	bool hovered = false;
};

#endif // !__W_SCENE_H__
