#include "M_Scene.h"

#include "Application.h"
#include "M_Renderer3D.h"
#include "M_Editor.h"
#include "M_Resources.h"

#include "SceneImporter.h"

#include "GameObject.h"
#include "C_Mesh.h"
#include "C_Material.h"
#include "C_Camera.h"
#include "C_PointLight.h"
#include "C_ProceduralMesh.h"
#include "C_PointLight.h"
#include "C_Aberration.h"
#include "C_Portal.h"

#include <stack>
#include <map>

M_Scene::M_Scene(bool start_enabled) : Module(start_enabled)
{
}


M_Scene::~M_Scene()
{
}


bool M_Scene::Start()
{
	/*App->resourceManager->DragAndDropImport("/Assets/water_plane.fbx", nullptr);
	gameObjects[0]->transform.SetEscale(0.1, 0.1, 0.1);
	gameObjects[0]->transform.SetPos(0, 4, 100);
	App->resourceManager->DragAndDropImport("/Assets/waterShader.txt", gameObjects[0]->childs[0]);

	App->resourceManager->DragAndDropImport("/Assets/street/Street environment_V01.FBX", nullptr);
	
	GameObject* object = AddLight();
	object->transform.SetPos(-150, 30, 0);*/

	return true;
}


UPDATE_STATUS M_Scene::PreUpdate(float dt)
{
	PreUpdateGameObjects(dt);

	return UPDATE_STATUS::UPDATE_CONTINUE;
}


UPDATE_STATUS M_Scene::Update(float dt)
{
	UpdateGameObjects(dt);

	return UPDATE_STATUS::UPDATE_CONTINUE;
}


UPDATE_STATUS M_Scene::PostUpdate(float dt)
{
	CheckObjectsToDelete();

	return UPDATE_STATUS::UPDATE_CONTINUE;
}


bool M_Scene::CleanUp()
{
	DeleteAllGameObjects();

	return true;
}


//TODO: for now, we assume that added game objects dont have a parent
void M_Scene::AddGameObject(GameObject* object)
{
	gameObjects.push_back(object);
}


void M_Scene::AddEmpty()
{
	GameObject* object = new GameObject(nullptr);
	object->SetName("Empty");

	gameObjects.push_back(object);
}


void M_Scene::AddCamera()
{
	GameObject* object = new GameObject(nullptr);
	object->AddComponent(new C_Camera());
	object->SetName("Camera");

	gameObjects.push_back(object);
}


GameObject* M_Scene::AddLight()
{
	GameObject* object = new GameObject(nullptr);
	object->AddComponent(new C_PointLight());
	object->SetName("Light");

	gameObjects.push_back(object);

	return object;
}


void M_Scene::AddProceduralMesh()
{
	GameObject* object = new GameObject(nullptr);
	object->AddComponent(new C_ProceduralMesh());
	object->SetName("Procedural mesh");

	gameObjects.push_back(object);
}


void M_Scene::AddAberration()
{
	GameObject* object = new GameObject(nullptr);
	object->AddComponent(new C_Aberration());
	object->SetName("Aberration");

	gameObjects.push_back(object);
}


void M_Scene::AddPortal()
{
	GameObject* object = new GameObject(nullptr);
	object->AddComponent(new C_Portal());
	object->SetName("Portal");

	gameObjects.push_back(object);
}


void M_Scene::NotifyShaderWasUpdated(int updatedShader)
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

			Component* component = node->GetComponent(COMPONENT_TYPE::MATERIAL);
			if (component != nullptr)
			{
				C_Material* material = (C_Material*)component;
				
				if (material->GetShader() == updatedShader)
					material->NotifyShaderUpdate();
			}


			if (node->childs.empty() == false)
			{
				childCount = node->childs.size();
				for (int j = 0; j < childCount; j++)
					stack.push(node->childs[j]);
			}
		}
	}
}


GameObject* M_Scene::GetGameObject(int uid)
{
	std::stack<GameObject*> stack;
	GameObject* node;

	int gameObjCount = gameObjects.size();
	for (int i = 0; i < gameObjCount; i++)
	{
		stack.push(gameObjects[i]);

		while (stack.empty() == false)
		{
			node = stack.top();
			stack.pop();

			if (node->GetUuid() == uid)
				return node;


			if (node->childs.empty() == false)
			{
				int childCount = node->childs.size();
				for (int j = 0; j < childCount; j++)
					stack.push(node->childs[j]);
			}
		}
	}

	return nullptr;
}


void M_Scene::GetGameObjectVector(std::vector<GameObject*>& vec)
{
	vec = gameObjects;
}


void M_Scene::CullGameObjects(std::vector<GameObject*>& objVector)
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

			Component* component = node->GetComponent(COMPONENT_TYPE::MESH);
			if (component != nullptr)
			{
				C_Mesh* mesh = (C_Mesh*)component;
				if (App->renderer3D->GetCurrentCamera()->IsInsideFrustum(mesh->GetAABB()) == true)
					objVector.push_back(node);
			}


			if (node->childs.empty() == false)
			{
				childCount = node->childs.size();
				for (int j = 0; j < childCount; j++)
					stack.push(node->childs[j]);
			}
		}
	}
}


void M_Scene::SaveScene()
{
	std::vector<GameObject*> vec;
	GetAllGameObjects(vec);

	SceneImporter::Save("test", vec);
}


void M_Scene::LoadScene()
{
	DeleteAllGameObjects();

	SceneImporter::Load("test", gameObjects);
}


void M_Scene::OnGameStart()
{
	std::vector<GameObject*> vec;
	GetAllGameObjects(vec);

	SceneImporter::Save("before_play", vec);
}


void M_Scene::OnGameEnd()
{
	DeleteAllGameObjects();

	SceneImporter::Load("before_play", gameObjects);
}


void M_Scene::GetAllGameObjects(std::vector<GameObject*>& vector) const
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

			vector.push_back(node);

			if (node->childs.empty() == false)
			{
				childCount = node->childs.size();
				for (int j = 0; j < childCount; j++)
					stack.push(node->childs[j]);
			}
		}
	}
}


void M_Scene::PreUpdateGameObjects(float dt)
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

			node->PreUpdate(dt);

			if (node->childs.empty() == false)
			{
				childCount = node->childs.size();
				for (int j = 0; j < childCount; j++)
					stack.push(node->childs[j]);
			}
		}
	}
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
					stack.push(node->childs[j]);
			}
		}
	}
}


void M_Scene::CheckObjectsToDelete()
{
	std::stack<GameObject*> stack;
	GameObject* node = nullptr;

	for (int i = 0; i < gameObjects.size(); i++)
	{
		if (gameObjects[i]->toDelete == true)
		{
			delete gameObjects[i];
			gameObjects.erase(gameObjects.begin() + i);
			i--;
		}

		else
		{
			stack.push(gameObjects[i]);

			while (stack.empty() == false)
			{
				node = stack.top();
				stack.pop();

				if (node->childs.empty() == false)
				{
					for (int j = 0; j < node->childs.size(); j++)
					{
						if (node->childs[j]->toDelete == true)
						{
							delete node->childs[j];
							node->childs.erase(node->childs.begin() + j);
							j--;
						}

						else
							stack.push(node->childs[j]);
					}
				}
			}
		}
	}
}


void M_Scene::TestRayCollision(LineSegment& ray)
{
	std::stack<GameObject*> stack;

	std::map<float, GameObject*> candidates;

	int gameObjCount = gameObjects.size();
	for (int i = 0; i < gameObjCount; i++)
	{
		stack.push(gameObjects[i]);

		while (stack.empty() == false)
		{
			GameObject* node = stack.top();
			stack.pop();

			Component* component = node->GetComponent(COMPONENT_TYPE::MESH);
			if (component != nullptr)
			{
				float distance;
				C_Mesh* mesh = (C_Mesh*)component;

				if (mesh->TestAABBRayCollision(ray, distance) == true)
					candidates.insert(std::pair<float, GameObject*>(distance, node));
			}

			if (node->childs.empty() == false)
			{
				int childCount = node->childs.size();

				for (int j = 0; j < childCount; j++)
					stack.push(node->childs[j]);
			}
		}
	}

	if (candidates.size() == 0)
		App->editor->SetFocusedGameObject(nullptr);

	else
	{
		std::map<float, GameObject*>::iterator it = candidates.begin();
		std::pair<float, GameObject*> selected(INFINITY, nullptr);

		for (it; it != candidates.end(); it++)
		{
			C_Mesh* mesh = (C_Mesh*)it->second->GetComponent(COMPONENT_TYPE::MESH);
			float distance = mesh->TestTriangleCollision(ray, it->second->transform.GetMatTransformT());

			if (distance != 0 && distance < selected.first)
			{
				selected.first = distance;
				selected.second = it->second;
			}
		}
		App->editor->SetFocusedGameObject(selected.second);
	}
}


void M_Scene::DeleteAllGameObjects()
{
	for (int i = 0; i < gameObjects.size(); i++)
	{
		delete gameObjects[i];
		gameObjects[i] = nullptr;

		gameObjects.erase(gameObjects.begin() + i);
		i--;
	}

	gameObjects.clear();
}