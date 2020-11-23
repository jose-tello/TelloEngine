#include "W_ObjectHierarchy.h"

#include "W_Inspector.h"

#include "Application.h"
#include "M_Scene.h"
#include "M_Editor.h"

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

	int flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanFullWidth;

	int gameObjCount = gameObjects.size();
	for (int i = 0; i < gameObjCount; i++)
	{
		bool open = ImGui::TreeNodeEx(gameObjects[i]->GetName(), flags);
		
		if (ImGui::IsItemClicked())
		{
			W_Inspector* inspector = (W_Inspector*)App->editor->GetWindow((int)E_WINDOW_TYPE::INSPECTOR);
			inspector->SetFocusedObject(gameObjects[i]);
		}


		
		if (open == true)
		{
			DrawChildren(gameObjects[i]->childs);
			ImGui::TreePop();
		}		
	}

	ImGui::End();

	return true;
}


void W_ObjectHierarchy::DrawChildren(std::vector<GameObject*>& vec)
{
	int flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanFullWidth;

	for (int i = 0; i < vec.size(); i++)
	{
		bool open = ImGui::TreeNodeEx(vec[i]->GetName(), flags);
		
		if (ImGui::IsItemClicked())
		{
			W_Inspector* inspector = (W_Inspector*)App->editor->GetWindow((int)E_WINDOW_TYPE::INSPECTOR);
			inspector->SetFocusedObject(vec[i]);
		}

		HandleDragAndDrop(vec[i]);

		if (open == true)
		{
			DrawChildren(vec[i]->childs);
			ImGui::TreePop();
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

	//TODO: make it maintain the transform
	//sceneChild->transform.NotifyNeedUpdate();
}