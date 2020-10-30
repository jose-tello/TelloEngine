#include "W_ObjectHierarchy.h"

#include "W_Inspector.h"

#include "Application.h"
#include "M_Scene.h"
#include "M_Editor.h"

#include "GameObject.h"

#include "imgui/imgui.h"

#include <vector>


W_ObjectHierarchy::W_ObjectHierarchy(bool open) : E_Window(open)
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
			W_Inspector* inspector = (W_Inspector*)App->editor->GetWindow(E_WINDOW_TYPE::INSPECTOR);
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

	int gameObjCount = vec.size();
	for (int i = 0; i < gameObjCount; i++)
	{
		bool open = ImGui::TreeNodeEx(vec[i]->GetName(), flags);
		
		if (ImGui::IsItemClicked())
		{
			W_Inspector* inspector = (W_Inspector*)App->editor->GetWindow(E_WINDOW_TYPE::INSPECTOR);
			inspector->SetFocusedObject(vec[i]);
		}

		if (open == true)
		{
			DrawChildren(vec[i]->childs);
			ImGui::TreePop();
		}
	}
}