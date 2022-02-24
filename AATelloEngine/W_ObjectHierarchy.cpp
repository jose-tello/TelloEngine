#include "W_ObjectHierarchy.h"

#include "W_Inspector.h"

#include "Application.h"
#include "M_Scene.h"
#include "M_Editor.h"
#include "M_Input.h"
#include "M_Renderer3D.h"

#include "GameObject.h"

#include "imgui/imgui.h"

#include <vector>


W_ObjectHierarchy::W_ObjectHierarchy(bool open) : E_Window(E_WINDOW_TYPE::GAME_OBJECTS, open)
{
}


W_ObjectHierarchy::~W_ObjectHierarchy()
{
}


bool W_ObjectHierarchy::Draw()
{
	ImGui::Begin("Hierarchy", &open);

	std::vector<GameObject*> gameObjects;
	App->scene->GetGameObjectVector(gameObjects);

	hovered = ImGui::IsWindowHovered();
	focused = ImGui::IsWindowFocused();

	int flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_DefaultOpen;

	bool open = ImGui::TreeNodeEx("Scene", flags);
		
	if (open == true)
	{
		DrawChildren(gameObjects);
		ImGui::TreePop();
	}		
	
	ImGui::End();

	return true;
}


void W_ObjectHierarchy::DrawChildren(std::vector<GameObject*>& vec)
{
	int flags = 0;

	for (int i = 0; i < vec.size(); i++)
	{
		if (vec[i]->childs.empty() == true)
			flags = ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_Leaf;

		else
			flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanFullWidth;

		bool open = ImGui::TreeNodeEx(vec[i]->GetName(), flags);
		
		if (ImGui::IsItemClicked())
		{
			W_Inspector* inspector = (W_Inspector*)App->editor->GetWindow((int)E_WINDOW_TYPE::INSPECTOR);
			inspector->SetFocusedObject(vec[i]);
		}

		if ((hovered == true || App->editor->IsWindowHovered(E_WINDOW_TYPE::SCENE_CAMERA)) &&
			(focused == true || App->editor->IsWindowFocused(E_WINDOW_TYPE::SCENE_CAMERA)) &&
			App->input->GetKey(BACKSPACE) == KEY_STATE::KEY_DOWN)
		{
			App->editor->DeleteFocusedObject();
			App->renderer3D->NotifyUpdateBuffers();
		}
		else
		{
			HandleDragAndDrop(vec[i]);

			if (open == true)
			{
				DrawChildren(vec[i]->childs);
				ImGui::TreePop();
			}
		}
	}
}


void W_ObjectHierarchy::HandleDragAndDrop(GameObject* currGameObject)
{
	//Drop
	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GameObject"))
		{
			GameObject* targetGo = (GameObject*)(payload->Data);

			if (targetGo != nullptr)
			{
				ReparentGameObjects(currGameObject, targetGo);
			}
		}
		ImGui::EndDragDropTarget();
	}

	//Drag
	else if (currGameObject != nullptr)
	{
		if (ImGui::BeginDragDropSource())
		{
			ImGui::SetDragDropPayload("GameObject", currGameObject, sizeof(GameObject));
			ImGui::EndDragDropSource();
		}
	}
}


void W_ObjectHierarchy::ReparentGameObjects(GameObject* parent, GameObject* child)
{
	GameObject* sceneChild = App->scene->GetGameObject(child->GetUuid());
	if (sceneChild->parent == parent || sceneChild == nullptr)
		return;
	

	sceneChild->parent->RemoveChild(child->GetUuid());
	sceneChild->parent = parent;

	if (parent == nullptr)
		App->scene->AddGameObject(sceneChild);
	
	else
		parent->childs.push_back(sceneChild);

	sceneChild->transform.SetGlobalTransform(sceneChild->transform.GetMatTransformT().Transposed());
}