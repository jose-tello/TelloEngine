#ifndef __M_CAMERA3D_H__
#define __M_CAMERA3D_H__

#include "Module.h"
#include "MathGeoLib/src/MathGeoLibFwd.h"

#define CAMERA_SPEED 1.f
#define MOUSE_SENSITIVITY 0.25f
#define MOUSE_ORBIT_SENSITIVITY 0.02f
#define MOUSE_WEEL_SPEED 25.f

class C_Camera;

class M_Camera3D : public Module
{
public:
	M_Camera3D(bool start_enabled = true);
	~M_Camera3D();

	bool Start() override;
	UPDATE_STATUS Update(float dt);

	float4x4 GetViewMatrix();
	float4x4 GetProjectionMatrix();
	float3 GetPosition();

	void Resize(float width, float height);
	void ClickSelect();

	//Getters Setters
	float GetNearPlaneDst() const;
	void SetNearPlaneDst(float);

	float GetFarPlaneDst() const;
	void SetFarPlaneDst(float);

	float GetVerticalFov() const;
	void SetVerticalFov(float fov);

	float GetAspectRatio() const;
	void SetAspectRatio(float aspRatio);
	
private:

	void MoveCamera();
	void RotateCamera();
	void ZoomCamera(int weelMotion);
	void MoveCameraSideways();
	void OrbitCamera(float3& reference);

public:
	bool drawClickRay;
	bool debugFrustumCull = false; //Must ALLWAYS be initialized as false
	bool drawAABB;

private:
	C_Camera* camera = nullptr;
};

#endif // !__M_CAMERA3D_H__