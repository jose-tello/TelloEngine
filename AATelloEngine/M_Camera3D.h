#ifndef __M_CAMERA3D_H__
#define __M_CAMERA3D_H__

#include "Module.h"
#include "glmath.h"

#define CAMERA_SPEED 30.f
#define MOUSE_SENSITIVITY 0.25f
#define MOUSE_WEEL_SPEED 250.f

class M_Camera3D : public Module
{
public:
	M_Camera3D(bool start_enabled = true);
	~M_Camera3D();

	UPDATE_STATUS Update(float dt);

	void Look(const vec3 &Position, const vec3 &Reference, bool RotateAroundReference = false);
	void LookAt(const vec3 &Spot);
	void Move(const vec3 &Movement);

	float* GetViewMatrix();
	
private:
	void CalculateViewMatrix();

	void MoveCamera(float dt);
	void ZoomCamera(int weelMotion, float dt);
	void MoveCameraSideways(float dt);
	void MoveCameraOrbital(float dt);

public:
	vec3 X, Y, Z, position, reference;

private:
	mat4x4 viewMatrix, viewMatrixInverse;
};

#endif // !__M_CAMERA3D_H__