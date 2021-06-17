#include "W_Scene.h"

#include "Application.h"
#include "M_Editor.h"
#include "M_Camera3D.h"
#include "M_Renderer3D.h"
#include "M_Input.h"

#include "GameObject.h"
#include "C_Transform.h"

#include "imgui/imgui.h"
#include "ImGuizmo/ImGuizmo.h"

#include "MathGeoLib/src/MathGeoLib.h"

W_Scene::W_Scene(bool active, C_Camera* camera) : W_CameraView(active, camera, E_WINDOW_TYPE::SCENE_CAMERA),
	gizmoOperation(ImGuizmo::OPERATION::TRANSLATE)
{
	ImGuizmo::Enable(true);
}


W_Scene::~W_Scene()
{
}


bool W_Scene::Draw()
{
	ImGui::Begin("Scene", &open);

	ImGui::BeginChild("Game render");
	ImVec2 size = ImGui::GetWindowSize();
	ImVec2 winPos = ImGui::GetWindowPos();
	ImVec2 mousePos = ImGui::GetMousePos();
	windowPosX = winPos.x;
	windowPosY = winPos.y;
	mousePosX = mousePos.x;
	mousePosY = mousePos.y;

	if (size.x != windowWidth || size.y != windowHeight)
		OnResize(size.x, size.y);
	
	App->renderer3D->DrawScene(frameBuffer, camera, !App->camera->debugFrustumCull, App->camera->drawAABB);
	ImGui::Image((ImTextureID)textureBuffer, ImVec2(windowWidth, windowHeight), ImVec2(0, 1), ImVec2(1, 0));
	
	HandleGizmo();

	if (ImGuizmo::IsUsing() == false)
		HandleInput();
	
	ImGui::EndChild();
	ImGui::End();

	return true;
}


bool W_Scene::IsWindowHovered() const
{
	return hovered;
}



void W_Scene::HandleInput()
{
	hovered = ImGui::IsItemHovered();

	if (hovered)
	{
		//TODO: this should be done using event manager
		if (ImGui::IsItemClicked(0))
			App->camera->ClickSelect();

		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_STATE::KEY_DOWN)
			gizmoOperation = ImGuizmo::OPERATION::TRANSLATE;

		if (App->input->GetKey(SDL_SCANCODE_E) == KEY_STATE::KEY_DOWN)
			gizmoOperation = ImGuizmo::OPERATION::ROTATE;

		if (App->input->GetKey(SDL_SCANCODE_R) == KEY_STATE::KEY_DOWN)
			gizmoOperation = ImGuizmo::OPERATION::SCALE;
	}
}


void W_Scene::HandleGizmo()
{
	GameObject* focusedGO = App->editor->GetFocusedGameObject();

	if (focusedGO != nullptr)
	{
		float4x4 viewMat;
		viewMat.Set(App->camera->GetViewMatrix());

		float4x4 projMat;
		projMat.Set(App->camera->GetProjectionMatrix());

		float4x4 objTransform = focusedGO->transform.GetMatTransformT();
		
		ImGuizmo::MODE mode = ImGuizmo::MODE::WORLD;

		ImGuizmo::SetDrawlist();
		ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

		if ((ImGuizmo::OPERATION)gizmoOperation == ImGuizmo::OPERATION::SCALE)
			mode = ImGuizmo::MODE::LOCAL;

		ImGuizmo::Manipulate(viewMat.ptr(), projMat.ptr(), (ImGuizmo::OPERATION)gizmoOperation, mode, objTransform.ptr());

		if (ImGuizmo::IsUsing())
			focusedGO->transform.SetGlobalTransform(objTransform.Transposed());
	}
}