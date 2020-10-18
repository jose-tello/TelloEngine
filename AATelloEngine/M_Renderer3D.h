#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"

class Mesh;

#define MAX_LIGHTS 8

class M_Renderer3D : public Module
{
public:
	M_Renderer3D(bool start_enabled = true);
	~M_Renderer3D();

	bool Init();
	UPDATE_STATUS PreUpdate(float dt) override;
	UPDATE_STATUS PostUpdate(float dt) override;
	bool CleanUp();

	void OnResize(float width, float height);

	void SetDepthTestEnabled(bool enable);
	void SetCullFaceEnabled(bool enable);
	void SetLightingEnabled(bool enable);
	void SetColorMatEnabled(bool enable);
	void SetTexture2DEnabled(bool enable);
	//void SetFillMode(bool enable);
	void SetWireframeMode(bool enable);


private:
	void GenerateFrameBuffer(float width, float height);
	
	void DrawSceneTexture();
	void DrawAllMeshes();

public:
	SDL_GLContext context;
	mat4x4 projectionMatrix;

	uint frameBuffer;
	uint textureBuffer;
	uint depthBuffer;

private:
	bool depthTestEnabled;
	bool cullFaceEnabled;
	bool lightingEnabled;
	bool colorMatEnabled;
	bool texture2DEnabled;
	bool wireframeModeEnabled;

	std::vector<Mesh*> meshVector;
};