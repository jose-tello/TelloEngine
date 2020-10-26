#include "E_ObjectHierarchy.h"

#include "Application.h"
#include "M_Scene.h"

#include "GameObject.h"

#include "imgui/imgui.h"

#include <vector>


E_ObjectHierarchy::E_ObjectHierarchy(bool open) : E_Window(open)
{

}


E_ObjectHierarchy::~E_ObjectHierarchy()
{

}


bool E_ObjectHierarchy::Draw()
{
	ImGui::Begin("Hierarchy", &open);

	std::vector<GameObject*> gameObjects;
	App->scene->GetGameObjectVector(gameObjects);

	int gameObjCount = gameObjects.size();
	for (int i = 0; i < gameObjCount; i++)
	{
		if (ImGui::TreeNodeEx(gameObjects[i]->GetName()))
		{
			DrawChildren(gameObjects[i]->childs);
			ImGui::TreePop();
		}
			
	}

	ImGui::End();

	return true;
}


void E_ObjectHierarchy::DrawChildren(std::vector<GameObject*>& vec)
{
	int gameObjCount = vec.size();
	for (int i = 0; i < gameObjCount; i++)
	{
		if (ImGui::TreeNodeEx(vec[i]->GetName()))
		{
			DrawChildren(vec[i]->childs);
			ImGui::TreePop();
		}
	}
}