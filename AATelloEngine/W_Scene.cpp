#include "W_Scene.h"

#include "Application.h"
#include "M_Editor.h"
#include "M_Camera3D.h"
#include "M_Renderer3D.h"

#include "GameObject.h"
#include "C_Transform.h"

#include "imgui/imgui.h"
#include "ImGuizmo/ImGuizmo.h"

#include "MathGeoLib/src/MathGeoLib.h"

W_Scene::W_Scene(bool active, C_Camera* camera) : W_CameraView(active, camera, E_WINDOW_TYPE::SCENE_CAMERA)
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

	if (size.x != windowWidth || size.y != windoHeight)
		OnResize(size.x, size.y);
	

	App->renderer3D->DrawScene(frameBuffer, camera, !App->camera->debugFrustumCull);
	ImGui::Image((ImTextureID)textureBuffer, ImVec2(windowWidth, windoHeight), ImVec2(0, 1), ImVec2(1, 0));
	
	HandleGizmo();

	if (ImGuizmo::IsUsing() == false)
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


void W_Scene::HandleGizmo()
{
	GameObject* focusedGO = App->editor->GetFocusedGameObject();

	if (focusedGO != nullptr)
	{
		float4x4 viewMat;
		viewMat.Set(App->camera->GetViewMatrix());

		float4x4 projMat;
		projMat.Set(App->camera->GetProjectionMatrix());

		float4x4 objTransform = focusedGO->transform.GetMatTransform();
		
		ImGuizmo::SetDrawlist();
		ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windoHeight);

		ImGuizmo::Manipulate(viewMat.ptr(), projMat.ptr(), ImGuizmo::OPERATION::TRANSLATE, ImGuizmo::MODE::LOCAL, objTransform.ptr());

		if (ImGuizmo::IsUsing())
		{
			//float3 pos = objTransform.Transposed().TranslatePart();
			//focusedGO->transform.SetPos(pos.x, pos.y, pos.z);
			focusedGO->transform.SetGlobalTransform(objTransform.Transposed());
		}
	}
}