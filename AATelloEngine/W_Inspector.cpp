#include "W_Inspector.h"

#include "GameObject.h"
#include "C_Mesh.h"
#include "C_Material.h"

#include "Application.h"
#include "M_Input.h"

#include "imgui/imgui.h"

W_Inspector::W_Inspector(bool open) : E_Window(open),
	focusedObject(nullptr)
{
}


W_Inspector::~W_Inspector()
{
	focusedObject = nullptr;
}


bool W_Inspector::Draw()
{
	if (App->input->GetKey(BACKSPACE) == KEY_STATE::KEY_DOWN)
		DeleteFocusedObject();
	

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


GameObject* W_Inspector::GetFocusedGameObject() const
{
	return focusedObject;
}


void W_Inspector::SetFocusedObject(GameObject* obj)
{
	focusedObject = obj;
}


void W_Inspector::QuitFocusedObject()
{
	focusedObject = nullptr;
}


bool W_Inspector::DeleteFocusedObject()
{
	if (focusedObject != nullptr)
		focusedObject->toDelete = true;

	focusedObject = nullptr;

	return true;
}


void W_Inspector::DrawGameObject(GameObject* obj)
{
	if (ImGui::CollapsingHeader("GameObject"), ImGuiTreeNodeFlags_DefaultOpen)
	{
		ImGui::Text("Name: ");	ImGui::SameLine(); ImGui::Text(obj->GetName());
	}

	ImGui::NewLine();
}


void W_Inspector::DrawTransformComp(C_Transform* transform)
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


void W_Inspector::DrawMeshComp(C_Mesh* mesh)
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


void W_Inspector::DrawMaterialComp(C_Material* material)
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