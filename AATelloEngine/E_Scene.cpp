#include "Application.h"
#include "E_Scene.h"

#include "M_Renderer3D.h"
#include "M_Window.h"

#include "imgui/imgui.h"

E_Scene::E_Scene(bool active) :
	E_Window(active),
	windowWidth(0),
	windoHeight(0)
{
}


E_Scene::~E_Scene()
{

}

bool E_Scene::Start() 
{
	return true;
}

bool E_Scene::Update()
{
	return true;
}

bool E_Scene::Draw()
{
	ImGui::Begin("Scene");

	ImGui::BeginChild("Game render");
	ImVec2 size = ImGui::GetWindowSize();

	if (size.x != windowWidth || size.y != windoHeight)
	{
		windowWidth = size.x;
		windoHeight = size.y;

		App->renderer3D->OnResize(windowWidth, windoHeight);
	}

	ImGui::Image((ImTextureID)App->renderer3D->textureBuffer, ImVec2(windowWidth, windoHeight), ImVec2(0, 1), ImVec2(1, 0));
	ImGui::EndChild();
	ImGui::End();

	return true;
}