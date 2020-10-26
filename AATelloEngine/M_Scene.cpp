#include "M_Scene.h"
#include "Application.h"

#include "GameObject.h"
#include "C_Mesh.h"
#include "C_Material.h"

#include <stack>

M_Scene::M_Scene(bool start_enabled) : Module(start_enabled)
{
}


M_Scene::~M_Scene()
{
	
}


bool M_Scene::Start() //TODO: Charge house here
{
	//Debug purposes

	/*GameObject* obj = new GameObject(nullptr);

	C_Mesh* mesh = new C_Mesh(obj);
	mesh->InitAsCube();

	obj->AddComponent(mesh);

	gameObjects.push_back(obj);*/

	return true;
}


UPDATE_STATUS M_Scene::Update(float dt)
{
	UpdateGameObjects(dt);

	return UPDATE_STATUS::UPDATE_CONTINUE;
}


UPDATE_STATUS M_Scene::PostUpdate(float dt)
{
	PostUpdateGameObjects(dt);

	return UPDATE_STATUS::UPDATE_CONTINUE;
}


bool M_Scene::CleanUp()
{
	int gameObjCount = gameObjects.size();
	for (int i = 0; i < gameObjCount; i++)
	{
		delete gameObjects[i];
		gameObjects[i] = nullptr;
	}

	gameObjects.clear();

	return true;
}


//TODO: for now, we assume that added game objects dont have a parent
void M_Scene::AddGameObject(GameObject* object)
{
	gameObjects.push_back(object);
}


void M_Scene::UpdateGameObjects(float dt)
{
	std::stack<GameObject*> stack;
	GameObject* node;

	int childCount;

	int gameObjCount = gameObjects.size();
	for (int i = 0; i < gameObjCount; i++)
	{
		stack.push(gameObjects[i]);

		while (stack.empty() == false)
		{
			node = stack.top();
			stack.pop();

			node->Update(dt);

			if (node->childs.empty() == false)
			{
				childCount = node->childs.size();
				for (int j = 0; j < childCount; j++)
				{
					stack.push(node->childs[j]);
				}
			}
		}
	}
}


void M_Scene::PostUpdateGameObjects(float dt)
{
	std::stack<GameObject*> stack;
	GameObject* node;

	int childCount;

	int gameObjCount = gameObjects.size();
	for (int i = 0; i < gameObjCount; i++)
	{
		stack.push(gameObjects[i]);

		while (stack.empty() == false)
		{
			node = stack.top();
			stack.pop();

			node->PostUpdate(dt);

			if (node->childs.empty() == false)
			{
				childCount = node->childs.size();
				for (int j = 0; j < childCount; j++)
				{
					stack.push(node->childs[j]);
				}
			}
		}
	}
}


void M_Scene::DrawGameObjects(bool drawVertexNormals, bool drawFaceNormals, bool black)
{
	std::stack<GameObject*> stack;
	GameObject* node;

	int childCount;

	int gameObjCount = gameObjects.size();
	for (int i = 0; i < gameObjCount; i++)
	{
		stack.push(gameObjects[i]);

		while (stack.empty() == false)
		{
			node = stack.top();
			stack.pop();

			if (node->GetComponent(COMPONENT_TYPE::MESH) != nullptr)
				DrawObject(node, drawVertexNormals, drawFaceNormals, black);
			

			if (node->childs.empty() == false)
			{
				childCount = node->childs.size();
				for (int j = 0; j < childCount; j++)
				{
					stack.push(node->childs[j]);
				}
			}
		}
	}
}


void M_Scene::DrawObject(GameObject* object, bool drawVertexNormals, bool drawFaceNormals, bool black)
{
	unsigned int texId = 0;
	float* color = nullptr;

	C_Mesh* mesh = (C_Mesh*)object->GetComponent(COMPONENT_TYPE::MESH);
	C_Material* material = (C_Material*)object->GetComponent(COMPONENT_TYPE::MATERIAL);

	if (material != nullptr)
	{
		Color col;
		material->GetDrawVariables(texId, col);
		color = &col;
	}

	if (black)
	{
		color = &Black;
	}


	mesh->Draw(object->transform.GetMatTransform().M, texId, color);
}