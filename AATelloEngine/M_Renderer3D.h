#ifndef __M_RENDERER3D_H__
#define __M_RENDERER3D_H__

#include "Module.h"
#include "Light.h"
#include "glmath.h"

#include "SDL\include\SDL.h"

#include <string>

class C_Camera;

class M_Renderer3D : public Module
{
public:
	M_Renderer3D(bool start_enabled = true);
	~M_Renderer3D() override;

	bool Init() override;
	UPDATE_STATUS PreUpdate(float dt) override;
	UPDATE_STATUS PostUpdate(float dt) override;
	bool CleanUp() override;

	void OnResize(float width, float height, C_Camera* camera);
	void GenerateFrameBuffer(float width, float height, unsigned int& frameBuffer, 
							 unsigned int& textureBuffer, unsigned int& depthBuffer);
	void DeleteBuffers(unsigned int frameBuffer, unsigned int textureBuffer, unsigned int depthBuffer);

	void DrawScene(unsigned int frameBuffer, C_Camera* camera);

	C_Camera* GetCurrentCamera();

	void SetDepthTestEnabled(bool enable);
	void SetCullFaceEnabled(bool enable);
	void SetLightingEnabled(bool enable);
	void SetColorMatEnabled(bool enable);
	void SetTexture2DEnabled(bool enable);
	void SetFillMode(bool enable);
	void SetWireframeMode(bool enable);

private:
	void DrawObjects();

public:
	SDL_GLContext context;

private:
	bool depthTestEnabled;
	bool cullFaceEnabled;
	bool lightingEnabled;
	bool colorMatEnabled;
	bool texture2DEnabled;
	bool fillModeEnabled;
	bool wireframeModeEnabled;

	Light light;

	//Camera we are rendering to, used to get the frustum planes to cull meshes,
	//WARNING: set to nullptr after drawing scene
	C_Camera* currentCamera = nullptr;
};

#endif // !__M_RENDERER3D_H__