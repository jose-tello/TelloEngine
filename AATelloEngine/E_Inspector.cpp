#include "E_Inspector.h"

#include "GameObject.h"
#include "C_Mesh.h"
#include "C_Material.h"

#include "imgui/imgui.h"

E_Inspector::E_Inspector(bool open) : E_Window(open),
	focusedObject(nullptr)
{

}


E_Inspector::~E_Inspector()
{

}


bool E_Inspector::Draw()
{
	ImGui::Begin("Inspector", &open);
	if (focusedObject != nullptr)
	{
		DrawGameObject(focusedObject);

		std::vector<Component*> componentsVec;
		focusedObject->GetAllComponents(componentsVec);

		int objCount = componentsVec.size();
		for (int i = 0; i < objCount; i++)
		{
			switch (componentsVec[i]->GetType())
			{
			case COMPONENT_TYPE::TRANSFORM:
				DrawTransformComp((C_Transform*)componentsVec[i]);
				break;

			case COMPONENT_TYPE::MESH:
				DrawMeshComp((C_Mesh*)componentsVec[i]);
				break;

			case COMPONENT_TYPE::MATERIAL:
				DrawMaterialComp((C_Material*)componentsVec[i]);
				break;

			default:

				break;
			}
		}
	}

	ImGui::End();

	return true;
}


void E_Inspector::SetFocusedObject(GameObject* obj)
{
	focusedObject = obj;
}


void E_Inspector::QuitFocusedObject()
{
	focusedObject = nullptr;
}


bool E_Inspector::DeleteFocusedObject()
{
	return true;
}


void E_Inspector::DrawGameObject(GameObject* obj)
{
	if (ImGui::CollapsingHeader("GameObject"), ImGuiTreeNodeFlags_DefaultOpen)
	{
		ImGui::Text("Name: ");	ImGui::SameLine(); ImGui::Text(obj->GetName());
	}
}


void E_Inspector::DrawTransformComp(C_Transform* transform)
{
	if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Text("Position: ");	ImGui::SameLine();
		ImGui::Spacing();
		ImGui::Spacing();

		ImGui::Text("Rotation: ");	ImGui::SameLine();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Text("Scale: ");
		ImGui::Spacing();
		ImGui::Spacing();
	}

}


void E_Inspector::DrawMeshComp(C_Mesh* mesh)
{
	if (ImGui::CollapsingHeader("Mesh", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Checkbox("Draw vertex normals", &mesh->drawVertexNormals);
		ImGui::Checkbox("Draw face normals", &mesh->drawFaceNormals);
	}
}


void E_Inspector::DrawMaterialComp(C_Material* material)
{

}