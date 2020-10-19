#ifndef __M_CAMERA3D_H__
#define __M_CAMERA3D_H__

#include "Module.h"
#include "glmath.h"

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

public:
	vec3 X, Y, Z, position, reference;

private:
	mat4x4 viewMatrix, viewMatrixInverse;
};

#endif // !__M_CAMERA3D_H__