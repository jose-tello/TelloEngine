#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"

class Primitive;
class Cube;
class Sphere;
class Piramid;

#define MAX_LIGHTS 8

class ModuleRenderer3D : public Module
{
public:
	ModuleRenderer3D(bool start_enabled = true);
	~ModuleRenderer3D();

	bool Init();
	UPDATE_STATUS PreUpdate(float dt) override;
	UPDATE_STATUS PostUpdate(float dt) override;
	bool CleanUp();

	void OnResize(int width, int height);

private:
	void GenerateFrameBuffer();
	void DrawSceneTexture();

public:
	SDL_GLContext context;
	mat3x3 NormalMatrix;
	mat4x4 ProjectionMatrix;

	uint frameBuffer;
	uint textureBuffer;
	uint depthBuffer;

	Cube* cube;
	Sphere* sphere;
	Piramid* piramid;
};