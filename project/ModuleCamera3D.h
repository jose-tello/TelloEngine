#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"

class btVector3;

class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D(Application* app, bool start_enabled = true);
	~ModuleCamera3D();

	bool Start();
	bool Update(float dt);
	bool CleanUp();

	void Look(const vec3 &Position, const vec3 &Reference, bool RotateAroundReference = false);
	void LookAt(const vec3 &Spot);
	void Move(const vec3 &Movement);

	float* GetViewMatrix();

	

private:

	void CalculateViewMatrix();

public:
	
	vec3 X, Y, Z, position, reference;
	
private:
	mat4x4 viewMatrix;

};