#include "W_Scene.h"

#include "Application.h"
#include "M_Renderer3D.h"
#include "M_Window.h"

#include "GameObject.h"
#include "C_Camera.h"

#include "imgui/imgui.h"

#include <string>

W_Scene::W_Scene(bool active, C_Camera* camera) : E_Window(active),
	windowWidth(0),
	windoHeight(0),

	frameBuffer(0),
	textureBuffer(0),
	depthBuffer(0),

	camera(camera)
{
}


W_Scene::~W_Scene()
{
	App->renderer3D->DeleteBuffers(frameBuffer, textureBuffer, depthBuffer);
	
	frameBuffer = 0;
	textureBuffer = 0;
	depthBuffer = 0;
}


bool W_Scene::Draw()
{
	std::string name;
	GameObject* cam = camera->GetOwner();

	if (cam != nullptr)
		name = cam->GetName();
	else
		name = "Scene";

	ImGui::Begin(name.c_str(), &open);

	ImGui::BeginChild("Game render");
	ImVec2 size = ImGui::GetWindowSize();

	if (size.x != windowWidth || size.y != windoHeight)
	{
		windowWidth = size.x;
		windoHeight = size.y;

		App->renderer3D->OnResize(windowWidth, windoHeight, camera);
		App->renderer3D->GenerateFrameBuffer(windowWidth, windoHeight, frameBuffer, textureBuffer, depthBuffer);
	}

	App->renderer3D->DrawScene(frameBuffer, camera);
	ImGui::Image((ImTextureID)textureBuffer, ImVec2(windowWidth, windoHeight), ImVec2(0, 1), ImVec2(1, 0));
	ImGui::EndChild();
	ImGui::End();

	return true;
}