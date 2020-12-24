#ifndef __M_RENDERER3D_H__
#define __M_RENDERER3D_H__

#include "Module.h"
#include "Light.h"

#include "SDL\include\SDL.h"

#include <vector>

class C_Camera;
class C_Mesh;
struct GameObject;

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

	void DrawScene(unsigned int frameBuffer, C_Camera* camera, bool pushCamera = true, bool drawAABB = false);
	void DrawCube(float*) const;

	C_Camera* GetCurrentCamera() const;

	void PushFrustum(C_Camera*);
	
	void SetCameraRay(float rayBegin[3], float rayEnd[3]);

	void SetDepthTestEnabled(bool enable);
	void SetCullFaceEnabled(bool enable);
	void SetLightingEnabled(bool enable);
	void SetColorMatEnabled(bool enable);
	void SetTexture2DEnabled(bool enable);
	void SetFillMode(bool enable);
	void SetWireframeMode(bool enable);

private:
	void PushCamera(C_Camera*);
	void PopCamera();

	void DrawFrustums() const;
	void DrawClickRay() const;
	void DrawObjects(C_Camera* camera, bool drawAABB) const;
	void DrawMesh(GameObject* object, C_Camera* camera, bool wireframeMode, bool drawAABB) const;

	void GetDrawVariables(GameObject* object, C_Mesh** meshPointer, unsigned int& textureId, Color& color, unsigned int& programId) const;
	void SetShaderUniforms(int programId, float* color, float* modelMat, C_Camera* camera, bool hasTexture) const;
	void HandleMeshDebugDraw(C_Mesh* mesh, bool drawAABB, float* transform) const;	//Handles aabb, vertex normals and face normals draw calls

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

	//Camera we are rendering to, used to frustum cull
	//WARNING: use pop / push fuctions
	C_Camera* currentCamera = nullptr;

	std::vector<C_Camera*> frustumVector;

	float cameraRay1[3];
	float cameraRay2[3];
};

#endif // !__M_RENDERER3D_H__