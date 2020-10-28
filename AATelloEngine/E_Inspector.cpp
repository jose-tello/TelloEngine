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


GameObject* E_Inspector::GetFocusedGameObject() const
{
	return focusedObject;
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

	ImGui::NewLine();
}


void E_Inspector::DrawTransformComp(C_Transform* transform)
{
	if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
	{
		float x, y, z, angle;

		transform->GetPos(x, y, z);
		float pos[] = { x, y, z };

		ImGui::Text("Position: ");
		ImGui::InputFloat3("", pos, 2, ImGuiInputTextFlags_AutoSelectAll);
		ImGui::NewLine();
		ImGui::Separator();

		transform->GetRotation(angle, x, y, z);
		float rotation[] = { x, y, z };

		ImGui::Text("Rotation: ");
		ImGui::InputFloat3("", rotation, 2, ImGuiInputTextFlags_AutoSelectAll);
		ImGui::InputFloat("Angle rotation:", &angle);
		ImGui::NewLine();
		ImGui::Separator();

		transform->GetEscale(x, y, z);
		float escale[] = { x, y, z };

		ImGui::Text("Scale: ");
		ImGui::InputFloat3("", escale, 2, ImGuiInputTextFlags_AutoSelectAll);
		ImGui::NewLine();
	}

	ImGui::NewLine();
}


void E_Inspector::DrawMeshComp(C_Mesh* mesh)
{
	if (ImGui::CollapsingHeader("Mesh", ImGuiTreeNodeFlags_DefaultOpen))
	{
		unsigned int verSize, normSize, indSize;
		mesh->GetAllVectorsSize(verSize, normSize, indSize);

		ImGui::Text("Vertices: %i", verSize);
		ImGui::Text("Normals: %i", normSize);
		ImGui::Text("Indices: %i", indSize);

		ImGui::NewLine();
		ImGui::Separator();

		ImGui::Checkbox("Draw vertex normals", &mesh->drawVertexNormals);
		ImGui::Checkbox("Draw face normals", &mesh->drawFaceNormals);
	}
	ImGui::NewLine();
}


void E_Inspector::DrawMaterialComp(C_Material* material)
{
	if (ImGui::CollapsingHeader("Material", ImGuiTreeNodeFlags_DefaultOpen))
	{
		bool texEnabled = material->GetTextureEnabled();
		bool colEnabled = material->GetColorEnabled();

		ImGui::Checkbox("Texture", &texEnabled);
		ImGui::Checkbox("Color", &colEnabled);

		material->SetTextureEnable(texEnabled);
		material->SetColorEnable(colEnabled);
	}

	ImGui::NewLine();
}