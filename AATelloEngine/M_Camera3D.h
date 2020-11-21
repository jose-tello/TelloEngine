#ifndef __M_CAMERA3D_H__
#define __M_CAMERA3D_H__

#include "Module.h"
#include "MathGeoLib/src/MathGeoLibFwd.h"

#define CAMERA_SPEED 30.f
#define MOUSE_SENSITIVITY 0.25f
#define MOUSE_WEEL_SPEED 250.f

class C_Camera;

class M_Camera3D : public Module
{
public:
	M_Camera3D(bool start_enabled = true);
	~M_Camera3D();

	bool Start() override;
	UPDATE_STATUS Update(float dt);

	float* GetViewMatrix();
	float* GetProjectionMatrix();
	float3 GetPosition();

	void Resize(float width, float height);
	
private:

	void MoveCamera(float dt);
	void RotateCamera(float dt);
	void ZoomCamera(int weelMotion, float dt);
	void MoveCameraSideways(float dt);

private:
	C_Camera* camera = nullptr;
};

#endif // !__M_CAMERA3D_H__