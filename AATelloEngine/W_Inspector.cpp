#include "W_Inspector.h"

#include "GameObject.h"
#include "C_Mesh.h"
#include "C_Material.h"
#include "C_Camera.h"

#include "Application.h"
#include "M_Input.h"

#include "imgui/imgui.h"

W_Inspector::W_Inspector(bool open) : E_Window(E_WINDOW_TYPE::INSPECTOR, open),
	focusedObject(nullptr),
	drawAddMenu(true)
{
}


W_Inspector::~W_Inspector()
{
	focusedObject = nullptr;
}


bool W_Inspector::Draw()
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

			case COMPONENT_TYPE::CAMERA:
				DrawCameraComp((C_Camera*)componentsVec[i]);
				break;

			default:
				assert("Forgot to add component");
				break;
			}
		}

		ImVec2 size = ImGui::GetWindowSize();
		size.y = ADD_HEIGHT;
		if (ImGui::Button("Add", size))
			drawAddMenu = true;

		if (drawAddMenu == true)
			DrawAddMenu(focusedObject);
	}
	else
		drawAddMenu = false;

	ImGui::End();

	return true;
}


GameObject* W_Inspector::GetFocusedGameObject() const
{
	return focusedObject;
}


bool W_Inspector::GetFocusedGameObjectPos(float& x, float& y, float& z) const
{
	if (focusedObject == nullptr)
		return false;

	focusedObject->transform.GetGlobalPos(x, y, z);
	return true;
}



void W_Inspector::SetFocusedObject(GameObject* obj)
{
	focusedObject = obj;
	drawAddMenu = false;
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
		DrawTransformPos(transform);

		DrawTransformRot(transform);

		DrawTransformScale(transform);
	}

	ImGui::NewLine();
}


void W_Inspector::DrawTransformPos(C_Transform* transform)
{
	float x, y, z;
	transform->GetPos(x, y, z);

	float pos[] = { x, y, z };

	if (ImGui::InputFloat3("Position", pos, 2, ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue))
		transform->SetPos(pos[0], pos[1], pos[2]);
	
	ImGui::NewLine();
	ImGui::Separator();
}


void W_Inspector::DrawTransformRot(C_Transform* transform)
{
	float x, y, z;

	transform->GetEulerAngles(x, y, z);
	float rotation[] = { x, y, z };

	if (ImGui::InputFloat3("Rotation", rotation, 2, ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue))
		transform->SetEulerAngles(rotation[0], rotation[1], rotation[2]);
	

	ImGui::NewLine();
	ImGui::Separator();
}


void W_Inspector::DrawTransformScale(C_Transform* transform)
{
	float x, y, z;

	transform->GetEscale(x, y, z);
	float escale[] = { x, y, z };

	if (ImGui::InputFloat3("Scale", escale, 2, ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue))
		transform->SetEscale(escale[0], escale[1], escale[2]);
	
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
		ImGui::Text("Texture Path: %s", material->GetTexturePath().c_str());
		int width, height;
		material->GetTextureSize(width, height);
		ImGui::Text("Tex width: %i", width);
		ImGui::Text("Tex height: %i", height);

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


void W_Inspector::DrawCameraComp(C_Camera* camera)
{
	if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen))
	{
		float aspectRatio = camera->GetAspectRatio();
		float verticalFov = camera->GetVerticalFov();
		float farDst = camera->GetFarPlaneDst();
		float nearDst = camera->GetNearPlaneDst();

		if (ImGui::InputFloat("Aspect ratio", &aspectRatio, ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue))
			camera->SetAspectRatio(aspectRatio);

		if (ImGui::InputFloat("Vertical fov", &verticalFov, ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue))
			camera->SetVerticalFov(verticalFov);

		if (ImGui::InputFloat("Far plane distance", &farDst, ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue))
			camera->SetFarPlaneDst(farDst);

		if (ImGui::InputFloat("Near plane distance", &nearDst, ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue))
			camera->SetNearPlaneDst(nearDst);
	}

	ImGui::NewLine();
}


//TODO: add components
void W_Inspector::DrawAddMenu(GameObject* gameObject)
{
	if (ImGui::Button("Add child"))
	{
		std::string name(gameObject->GetName());
		name.append(" child");

		GameObject* obj = new GameObject(name, gameObject);
		gameObject->childs.push_back(obj);

		drawAddMenu = false;
	}
}