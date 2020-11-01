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
		float auxPos[] = { x, y, z };

		if (ImGui::InputFloat3("Position", pos, 2, ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue))
		{
			x = pos[0] - auxPos[0];
			y = pos[1] - auxPos[1];
			z = pos[2] - auxPos[2];
			transform->SetPos(x, y, z);
		}
		ImGui::NewLine();
		ImGui::Separator();


		transform->GetRotation(angle, x, y, z);
		float rotation[] = { x, y, z };

		if (ImGui::InputFloat3("Rotation", rotation, 2, ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue));
		ImGui::Text("Angle rotation : ");
		ImGui::InputFloat("", &angle);
		ImGui::NewLine();
		ImGui::Separator();


		transform->GetEscale(x, y, z);
		float escale[] = { x, y, z };
		
		ImGui::Text("Scale: ");
		if (ImGui::InputFloat3("Scale", escale, 6, ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue));
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
		ImGui::Text("Texture Path: %s", material->texturePath.c_str());
		ImGui::Text("Tex width: %i", material->textureWidth);
		ImGui::Text("Tex height: %i", material->textureHeight);

		ImGui::Spacing();
		ImGui::Separator();

		bool texEnabled = material->GetTextureEnabled();
		bool colEnabled = material->GetColorEnabled();
		bool checkEnabled = material->GetCheckerTextureEnabled();

		ImGui::Checkbox("Texture", &texEnabled);
		ImGui::Checkbox("Color", &colEnabled);
		ImGui::Checkbox("Checker tex", &checkEnabled);

		material->SetTextureEnable(texEnabled);
		material->SetColorEnable(colEnabled);
		material->SetCheckerTextureEnable(checkEnabled);
	}

	ImGui::NewLine();
}