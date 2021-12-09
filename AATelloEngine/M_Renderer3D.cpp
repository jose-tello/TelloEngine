#include "Application.h"
#include "M_Renderer3D.h"
#include "M_Window.h"
#include "M_Editor.h"
#include "M_Scene.h"
#include "M_Camera3D.h"
#include "M_Resources.h"

#include "GameObject.h"

#include "C_Material.h"
#include "C_Mesh.h"
#include "C_Camera.h"
#include "C_PointLight.h"

#include "R_Shader.h"
#include "R_Mesh.h"

#include "Grid.h"
#include "UniformHandle.h"

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
vsync(true),
rasterizationRender(false),

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
		if (SDL_GL_SetSwapInterval(static_cast<int>(vsync)) < 0)
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
		//glEnable(GL_LIGHTING);
		glEnable(GL_COLOR_MATERIAL);
		glEnable(GL_TEXTURE_2D);
	}

	int workGroupSize[3];

	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, &workGroupSize[0]);
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 1, &workGroupSize[1]);
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 2, &workGroupSize[2]);

	App->editor->AddLog("WORK GROUP SIZE \n x: %i	y: %i	z: %i", workGroupSize[0], workGroupSize[1], workGroupSize[2]);

	int maxGroupInvocations;
	glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, &maxGroupInvocations);
	App->editor->AddLog("MAX GROUP INVOCATIONS: %i", maxGroupInvocations);

	return ret;
}


UPDATE_STATUS M_Renderer3D::PostUpdate(float dt)
{
	App->editor->Draw();
	SDL_GL_SwapWindow(App->window->window);

	lightVector.clear();
	frustumVector.clear();

	return UPDATE_STATUS::UPDATE_CONTINUE;
}


bool M_Renderer3D::CleanUp()
{
	App->editor->AddLog("Log: Destroying 3D Renderer");

	lightVector.clear();
	frustumVector.clear();

	SDL_GL_DeleteContext(context);

	return true;
}


void M_Renderer3D::OnResize(float width, float height, C_Camera* camera)
{
	camera->SetAspectRatio(width / height);
}


void M_Renderer3D::GenerateFrameBuffer(float width, float height, unsigned int& frameBuffer, unsigned int& textureBuffer, unsigned int& depthBuffer)
{
	glGenFramebuffers(1, &frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

	glGenTextures(1, &textureBuffer);
	glBindTexture(GL_TEXTURE_2D, textureBuffer);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_FLOAT, 0);
	glBindImageTexture(0, textureBuffer, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA8);

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
	glDeleteTextures(1, &textureBuffer);
	glDeleteRenderbuffers(1, &depthBuffer);
}


void M_Renderer3D::DrawScene(unsigned int frameBuffer, C_Camera* camera, int camWidth, int camHeight, bool pushCamera, bool drawAABB)
{

	glViewport(0, 0, camWidth, camHeight);
	if (pushCamera == true)
	{
		PopCamera();
		PushCamera(camera);
	}

	if (rasterizationRender == true)
	{
		RasterizationDraw(frameBuffer, camera, drawAABB);
	}

	else
		RayTracingDraw(frameBuffer, camera, camWidth, camHeight);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void M_Renderer3D::DrawCube(float* cube, float r, float g, float b) const
{
	glBegin(GL_LINES);

	glLineWidth(3.0f);
	glColor3f(r, g, b);

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


void M_Renderer3D::PushLight(C_PointLight* light)
{
	if (lightVector.size() < 4)
		lightVector.push_back(light);

	else
		App->editor->AddLog("[WARNING] Engine only supports up to 4 lights");
}


void M_Renderer3D::PushFrustum(C_Camera* frustum)
{
	frustumVector.push_back(frustum);
}


void M_Renderer3D::DeleteLight(C_PointLight* light)
{
	int lightCount = lightVector.size();
	for (int i = 0; i < lightCount; i++)
	{
		if (light == lightVector[i])
		{
			lightVector.erase(lightVector.begin() + i);
			break;
		}
	}
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


void M_Renderer3D::SetVsync(bool enable)
{
	if (vsync != enable)
	{
		vsync = enable;
		SDL_GL_SetSwapInterval(static_cast<int>(vsync));
	}
}


void M_Renderer3D::SetRasterization(bool enable)
{
	rasterizationRender = enable;
}


void M_Renderer3D::RayTracingDraw(unsigned int frameBuffer, C_Camera* camera, int winWidth, int winHeight)
{
	R_Shader* shader = static_cast<R_Shader*>(App->resourceManager->GetDefaultResource(DEFAULT_RESOURCE::RAY_TRACING_SHADER));

	std::vector<GameObject*> objToDraw;
	App->scene->CullGameObjects(objToDraw);


	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);

	shader->UseShaderProgram();
	
	
	unsigned int uniformLocation = glGetUniformLocation(shader->GetProgramId(), "projection");
	glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, camera->GetProjectionMat().ptr());

	uniformLocation = glGetUniformLocation(shader->GetProgramId(), "view");
	glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, camera->GetViewMat().ptr());
	
	GenerateArrayBuffers(shader->GetProgramId());

	glDispatchCompute(winWidth, winHeight, 1);

	// make sure writing to image has finished before read
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

	//DrawObjects(camera, drawAABB);	
}


void M_Renderer3D::GenerateArrayBuffers(unsigned int shaderId)
{
	std::vector<R_Mesh*> meshes = App->resourceManager->GetAllLoadedMeshes();

	std::vector<float> vertices;
	std::vector<unsigned int> indices;

	int indicesOffset = 0;

	int meshCount = meshes.size();

	for (int i = 0; i < meshCount; ++i)
	{
		std::vector<float> meshVertices = meshes[i]->GetVertices();
		std::vector<unsigned int> meshIndices = meshes[i]->GetIndices();
		
		vertices.insert(vertices.end(), meshVertices.begin(), meshVertices.end());
		indices.insert(indices.end(), meshIndices.begin(), meshIndices.end());

		meshes[i]->SetIndicesOffset(indicesOffset);
	}

	//do it uniform based
	unsigned int vertexTexture = 0;
	glGenTextures(1, &vertexTexture);
	glBindTexture(GL_TEXTURE_1D, vertexTexture);

	glTexImage1D(GL_TEXTURE_1D, 0, GL_RGB8, 3, 0, GL_RGB, GL_FLOAT, &vertices[0]);	//Width is indices size

	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	unsigned int texLocation = glGetUniformLocation(shaderId, "vertexTexture");
	glUniform1i(texLocation, 1);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_1D, vertexTexture);

}


void M_Renderer3D::PushCamera(C_Camera* cam)
{
	if (currentCamera == nullptr)
		currentCamera = cam;

	else
		assert("Pushing camera without poping the previous one");
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

void M_Renderer3D::RasterizationDraw(unsigned int frameBuffer, C_Camera* camera, bool drawAABB)
{
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


//TODO: Reorganize this
void M_Renderer3D::DrawMesh(GameObject* object, C_Camera* camera, bool wireframeMode, bool drawAABB) const
{
	C_Mesh* mesh;
	unsigned int texId = 0;
	Color color;
	unsigned int programId = 0;

	C_Material* material = GetDrawVariables(object, &mesh, texId, color, programId);

	if (wireframeMode == true)
		color = Black;

	glUseProgram(programId);

	bool hasTexture = false;
	if (texId != 0 && wireframeMode == false)
	{
		glBindTexture(GL_TEXTURE_2D, texId);
		hasTexture = true;
	}

	SetShaderUniforms(material, programId, &color, object->transform.GetMatTransformT(), camera, hasTexture);
	glBindVertexArray(mesh->GetVAO());

	glDrawElements(GL_TRIANGLES, mesh->GetIndicesSize(), GL_UNSIGNED_INT, NULL);

	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);
	glBindVertexArray(0);

	HandleMeshDebugDraw(mesh, drawAABB, object->transform.GetMatTransformT().ptr());
}


C_Material* M_Renderer3D::GetDrawVariables(GameObject* object, C_Mesh** meshPointer, unsigned int& textureId,
	Color& color, unsigned int& programId) const
{
	*meshPointer = (C_Mesh*)object->GetComponent(COMPONENT_TYPE::MESH);

	Component* mat = object->GetComponent(COMPONENT_TYPE::MATERIAL);
	if (mat != nullptr)
	{
		C_Material* material = (C_Material*)mat;
		material->GetDrawVariables(color, textureId, programId);

		return material;
	}

	return nullptr;
}


void M_Renderer3D::SetShaderUniforms(C_Material* material, int programId, float* color, float4x4& modelMat, C_Camera* camera, bool hasTexture) const
{
	if (material != nullptr)
	{
		UniformHandle* uniform = nullptr;

		uniform = material->GetUniform("material_color");

		if (uniform != nullptr)
			uniform->SetFloatVec3(color);

		uniform = material->GetUniform("model_matrix");

		if (uniform != nullptr)
			uniform->SetMat4(modelMat.ptr());

		uniform = material->GetUniform("projection");

		if (uniform != nullptr)
			uniform->SetMat4(camera->GetProjectionMat().ptr());

		uniform = material->GetUniform("view");

		if (uniform != nullptr)
			uniform->SetMat4(camera->GetViewMat().ptr());

		uniform = material->GetUniform("has_texture");

		if (uniform != nullptr)
			uniform->SetBool(hasTexture);

		uniform = material->GetUniform("timer");

		if (uniform != nullptr)
			uniform->SetFloat(App->GetTimeManager()->GetTimeSinceStart());

		int lightVecSize = lightVector.size();
		for (int i = 0; i < lightVecSize; ++i)
		{
			lightVector[i]->PushLightUniforms(material, i);
		}

		uniform = material->GetUniform("light_number");

		if (uniform != nullptr)
		{
			uniform->SetInt(lightVecSize);
		}

		uniform = material->GetUniform("view_position");

		if (uniform != nullptr)
		{
			float camPos[3];

			GameObject* cam = camera->GetOwner();

			if (cam != nullptr)
				cam->transform.GetPos(camPos[0], camPos[1], camPos[2]);
			else
			{
				float3 cameraPosition = App->camera->GetPosition();
				camPos[0] = cameraPosition.x;
				camPos[1] = cameraPosition.y;
				camPos[2] = cameraPosition.z;
			}

			uniform->SetFloatVec3(camPos);
		}

		material->SetUniformsToShader();
	}

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