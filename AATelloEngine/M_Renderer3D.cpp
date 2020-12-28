#include "Application.h"
#include "M_Renderer3D.h"
#include "M_Window.h"
#include "M_Editor.h"
#include "M_Scene.h"
#include "M_Camera3D.h"
#include "M_Resources.h"

#include "GameObject.h"

#include "C_Camera.h"
#include "C_Material.h"
#include "C_Mesh.h"

#include "Grid.h"
#include "MathGeoLib/src/MathGeoLib.h"

#include "Glew/include/glew.h"
#pragma comment(lib,"Glew/libx86/glew32.lib")

#include "SDL\include\SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */

M_Renderer3D::M_Renderer3D(bool start_enabled) : Module(start_enabled),
	context(),

	depthTestEnabled(true),
	cullFaceEnabled(true),
	lightingEnabled(true),
	colorMatEnabled(true),
	texture2DEnabled(true),
	fillModeEnabled(true),
	wireframeModeEnabled(false),

	currentCamera(nullptr),
	cameraRay1{ 0, 0, 0 },
	cameraRay2{ 0, 0, 0 }
{
}

// Destructor
M_Renderer3D::~M_Renderer3D()
{}


// Called before render is available
bool M_Renderer3D::Init()
{
	App->editor->AddLog("Log: Creating 3D Renderer context");
	bool ret = true;

	//Create context
	context = SDL_GL_CreateContext(App->window->window);
	if (context == NULL)
	{
		App->editor->AddLog("[ERROR]: OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		GLenum glewError = glewInit();

		if (glewError != GLEW_OK)
		{
			App->editor->AddLog("[ERROR]: Error initializing GLEW! %s\n", glewGetErrorString(glewError));
		}
	}

	if (ret == true)
	{
		//Use Vsync
		if (VSYNC && SDL_GL_SetSwapInterval(1) < 0)
			App->editor->AddLog("[ERROR]: Unable to set VSync! SDL Error: %s\n", SDL_GetError());

		//Check for error
		GLenum error = glGetError();
		if (error != GL_NO_ERROR)
		{
			App->editor->AddLog("[ERROR]: Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glEnable(GL_LIGHTING);
		glEnable(GL_COLOR_MATERIAL);
		glEnable(GL_TEXTURE_2D);
	}

	return ret;
}


UPDATE_STATUS M_Renderer3D::PreUpdate(float dt)
{
	light.SetPos(0, 40, 0);
	timer += 0.16;

	return UPDATE_STATUS::UPDATE_CONTINUE;
}


UPDATE_STATUS M_Renderer3D::PostUpdate(float dt)
{
	light.Render();
	App->editor->Draw();
	SDL_GL_SwapWindow(App->window->window);

	frustumVector.clear();

	return UPDATE_STATUS::UPDATE_CONTINUE;
}


bool M_Renderer3D::CleanUp()
{
	App->editor->AddLog("Log: Destroying 3D Renderer");

	SDL_GL_DeleteContext(context);

	return true;
}


void M_Renderer3D::OnResize(float width, float height, C_Camera* camera)
{
	glViewport(0, 0, width, height);

	camera->SetAspectRatio(width / height);
}


void M_Renderer3D::GenerateFrameBuffer(float width, float height, unsigned int& frameBuffer, unsigned int& textureBuffer, unsigned int& depthBuffer)
{
	glGenFramebuffers(1, &frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

	glGenTextures(1, &textureBuffer);
	glBindTexture(GL_TEXTURE_2D, textureBuffer);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenRenderbuffers(1, &depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);

	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureBuffer, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void M_Renderer3D::DeleteBuffers(unsigned int frameBuffer, unsigned int textureBuffer, unsigned int depthBuffer)
{
	glDeleteFramebuffers(1, &frameBuffer);
	glDeleteFramebuffers(1, &textureBuffer);
	glDeleteFramebuffers(1, &depthBuffer);
}


void M_Renderer3D::DrawScene(unsigned int frameBuffer, C_Camera* camera, bool pushCamera, bool drawAABB)
{
	if (pushCamera == true)
	{
		PopCamera();
		PushCamera(camera);
	}

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glLoadMatrixf(camera->GetProjectionMat().ptr());

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glLoadMatrixf(camera->GetViewMat().ptr());

	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);

	if (wireframeModeEnabled == true)
		glLineWidth(3.0f);

	DrawObjects(camera, drawAABB);

	if (drawAABB == true)
		DrawFrustums();

	Grid grid;
	grid.Draw();

	if (App->camera->drawClickRay == true)
		DrawClickRay();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void M_Renderer3D::DrawCube(float* cube) const
{
	glBegin(GL_LINES);

	glLineWidth(3.0f);
	glColor3f(0, 0.2f, 0.9f);

	glVertex3f(cube[0], cube[1], cube[2]);
	glVertex3f(cube[12], cube[13], cube[14]);
	glVertex3f(cube[0], cube[1], cube[2]);
	glVertex3f(cube[3], cube[4], cube[5]);
	glVertex3f(cube[0], cube[1], cube[2]);
	glVertex3f(cube[6], cube[7], cube[8]);
	glVertex3f(cube[6], cube[7], cube[8]);
	glVertex3f(cube[9], cube[10], cube[11]);
	glVertex3f(cube[3], cube[4], cube[5]);
	glVertex3f(cube[9], cube[10], cube[11]);
	glVertex3f(cube[12], cube[13], cube[14]);
	glVertex3f(cube[15], cube[16], cube[17]);
	glVertex3f(cube[12], cube[13], cube[14]);
	glVertex3f(cube[18], cube[19], cube[20]);
	glVertex3f(cube[15], cube[16], cube[17]);
	glVertex3f(cube[21], cube[22], cube[23]);
	glVertex3f(cube[18], cube[19], cube[20]);
	glVertex3f(cube[21], cube[22], cube[23]);
	glVertex3f(cube[6], cube[7], cube[8]);
	glVertex3f(cube[18], cube[19], cube[20]);
	glVertex3f(cube[3], cube[4], cube[5]);
	glVertex3f(cube[15], cube[16], cube[17]);
	glVertex3f(cube[9], cube[10], cube[11]);
	glVertex3f(cube[21], cube[22], cube[23]);

	glEnd();
}


C_Camera* M_Renderer3D::GetCurrentCamera() const
{
	return currentCamera;
}


void M_Renderer3D::PushFrustum(C_Camera* frustum)
{
	frustumVector.push_back(frustum);
}


void M_Renderer3D::SetCameraRay(float rayBegin[3], float rayEnd[3])
{
	cameraRay1[0] = rayBegin[0];
	cameraRay1[1] = rayBegin[1];
	cameraRay1[2] = rayBegin[2];

	cameraRay2[0] = rayEnd[0];
	cameraRay2[1] = rayEnd[1];
	cameraRay2[2] = rayEnd[2];
}


void M_Renderer3D::SetDepthTestEnabled(bool enable)
{
	if (depthTestEnabled != enable)
	{
		if (enable == true)
			glEnable(GL_DEPTH_TEST);

		else
			glDisable(GL_DEPTH_TEST);

		depthTestEnabled = enable;
	}
}


void M_Renderer3D::SetCullFaceEnabled(bool enable)
{
	if (cullFaceEnabled != enable)
	{
		if (enable == true)
			glEnable(GL_CULL_FACE);

		else
			glDisable(GL_CULL_FACE);

		cullFaceEnabled = enable;
	}
}


void M_Renderer3D::SetLightingEnabled(bool enable)
{
	if (lightingEnabled != enable)
	{
		if (enable == true)
			glEnable(GL_LIGHTING);

		else
			glDisable(GL_LIGHTING);

		lightingEnabled = enable;
	}
}


void M_Renderer3D::SetColorMatEnabled(bool enable)
{
	if (colorMatEnabled != enable)
	{
		if (enable == true)
			glEnable(GL_COLOR_MATERIAL);

		else
			glDisable(GL_COLOR_MATERIAL);

		colorMatEnabled = enable;
	}
}


void M_Renderer3D::SetTexture2DEnabled(bool enable)
{
	if (texture2DEnabled != enable)
	{
		if (enable == true)
			glEnable(GL_TEXTURE_2D);

		else
			glDisable(GL_TEXTURE_2D);

		texture2DEnabled = enable;
	}
}


void M_Renderer3D::SetFillMode(bool enable)
{
	if (fillModeEnabled != enable)
	{
		fillModeEnabled = enable;
	}
}


void M_Renderer3D::SetWireframeMode(bool enable)
{
	if (wireframeModeEnabled != enable)
	{
		wireframeModeEnabled = enable;
	}
}


void M_Renderer3D::PushCamera(C_Camera* cam)
{
	if (currentCamera == nullptr)
		currentCamera = cam;

	else
		assert("Pushing camera without poping the precious one");
}


void M_Renderer3D::PopCamera()
{
	currentCamera = nullptr;
}


void M_Renderer3D::DrawFrustums() const
{
	int frustumCount = frustumVector.size();
	for (int i = 0; i < frustumCount; i++)
	{
		frustumVector[i]->DrawFrustum();
	}
}


void M_Renderer3D::DrawClickRay() const
{
	glBegin(GL_LINES);

	glLineWidth(1.0f);
	glColor3f(1, 0, 0);

	glVertex3f(cameraRay1[0], cameraRay1[1], cameraRay1[2]);
	glVertex3f(cameraRay2[0], cameraRay2[1], cameraRay2[2]);

	glEnd();
}


void M_Renderer3D::DrawObjects(C_Camera* camera, bool drawAABB) const
{
	std::vector<GameObject*> objToDraw;
	App->scene->CullGameObjects(objToDraw);

	int objectsCount = objToDraw.size();
	for (int i = 0; i < objectsCount; i++)
	{
		if (fillModeEnabled == true)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			DrawMesh(objToDraw[i], camera, false, drawAABB);
		}

		if (wireframeModeEnabled == true)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			DrawMesh(objToDraw[i], camera, true, drawAABB);
		}
	}
}


void M_Renderer3D::DrawMesh(GameObject* object, C_Camera* camera, bool wireframeMode, bool drawAABB) const
{
	C_Mesh* mesh;
	unsigned int texId = 0;
	Color color;
	unsigned int programId = 0;

	GetDrawVariables(object, &mesh, texId, color, programId);

	if (wireframeMode == true)
		color = Black;

	glUseProgram(programId);

	bool hasTexture = false;
	if (texId != 0 && wireframeMode == false)
	{
		glBindTexture(GL_TEXTURE_2D, texId);
		hasTexture = true;
	}

	SetShaderUniforms(programId, &color, object->transform.GetMatTransformT(), camera, hasTexture);
	glBindVertexArray(mesh->GetVAO());

	glDrawElements(GL_TRIANGLES, mesh->GetIndicesSize(), GL_UNSIGNED_INT, NULL);

	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);
	glBindVertexArray(0);

	HandleMeshDebugDraw(mesh, drawAABB, object->transform.GetMatTransformT().ptr());
}


void M_Renderer3D::GetDrawVariables(GameObject* object, C_Mesh** meshPointer, unsigned int& textureId,
									Color& color, unsigned int& programId) const
{
	*meshPointer = (C_Mesh*)object->GetComponent(COMPONENT_TYPE::MESH);

	Component* mat = object->GetComponent(COMPONENT_TYPE::MATERIAL);
	if (mat != nullptr)
	{
		C_Material* material = (C_Material*)mat;
		material->GetDrawVariables(color, textureId, programId);
	}
}


void M_Renderer3D::SetShaderUniforms(int programId, float* color, float4x4& modelMat, C_Camera* camera, bool hasTexture) const
{
	unsigned int colorUniform = glGetUniformLocation(programId, "material_color");
	glUniform3fv(colorUniform, 1, color);

	unsigned int modelMatUniform = glGetUniformLocation(programId, "model_matrix");
	glUniformMatrix4fv(modelMatUniform, 1, GL_FALSE, modelMat.ptr());

	unsigned int projMat = glGetUniformLocation(programId, "projection");
	glUniformMatrix4fv(projMat, 1, GL_FALSE, camera->GetProjectionMat().ptr());

	unsigned int viewMat = glGetUniformLocation(programId, "view");
	glUniformMatrix4fv(viewMat, 1, GL_FALSE, camera->GetViewMat().ptr());

	unsigned int hasTextureUniform = glGetUniformLocation(programId, "has_texture");
	glUniform1i(hasTextureUniform, hasTexture);

	unsigned int timeUniform = glGetUniformLocation(programId, "timer");
	glUniform1f(timeUniform, App->GetTimeManager()->GetTimeSinceStart());
}


//Handles aabb, vertex normals and face normals draw calls
void M_Renderer3D::HandleMeshDebugDraw(C_Mesh* mesh, bool drawAABB, float* transform) const
{
	if (drawAABB == true)
		mesh->DrawAABB();

	glPushMatrix();
	glMultMatrixf(transform);
	mesh->HandleDebugDraws();
	glPopMatrix();
}