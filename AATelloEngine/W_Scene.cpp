#include "W_Scene.h"

#include "Application.h"
#include "M_Camera3D.h"
#include "M_Renderer3D.h"

#include "imgui/imgui.h"

W_Scene::W_Scene(bool active, C_Camera* camera) : W_CameraView(active, camera, E_WINDOW_TYPE::SCENE_CAMERA)
{
}


W_Scene::~W_Scene()
{
}


bool W_Scene::Draw()
{
	ImGui::Begin("Scene", &open);

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
	
	HandleInput();

	ImGui::EndChild();
	ImGui::End();

	return true;
}


void W_Scene::HandleInput()
{
	if (ImGui::IsItemHovered() == true)
	{
		//TODO: this should be done using event manager
		if (ImGui::IsItemClicked(0))
			App->camera->ClickSelect();
	}
}