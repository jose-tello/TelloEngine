#include "M_Scene.h"

#include "Application.h"
#include "M_FileManager.h"
#include "SceneImporter.h"

#include "GameObject.h"
#include "C_Mesh.h"
#include "Mesh.h"
#include "C_Material.h"
#include "C_Camera.h"

#include "Globals.h"

#include <stack>

M_Scene::M_Scene(bool start_enabled) : Module(start_enabled)
{
}


M_Scene::~M_Scene()
{
	
}


bool M_Scene::Start()
{
	App->fileManager->LoadFromExporter("/Assets/house/BakerHouse.fbx");

	float4x4 transform = float4x4::FromTRS(float3(0, 0, 0), Quat::FromEulerXYZ(90 * DEGTORAD, 0, 0),
										   float3(0.05f, 0.05f, 0.05f));

	gameObjects[0]->transform.AddTransform(transform);

	return true;
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


void M_Scene::AddPrimitive(PRIMITIVE_TYPE type)
{
	GameObject* object = new GameObject(nullptr);
	C_Mesh* meshComponent = new C_Mesh();
	Mesh* mesh = new Mesh();

	switch (type)
	{
	case PRIMITIVE_TYPE::CUBE:
		object->SetName("Cube");
		mesh->InitAsCube();
		break;

	case PRIMITIVE_TYPE::PIRAMID:
		object->SetName("Piramid");
		mesh->InitAsPiramid();
		break;

	case PRIMITIVE_TYPE::SPHERE:
		object->SetName("Sphere");
		mesh->InitAsSphere();
		break;

	case PRIMITIVE_TYPE::CILINDER:
		object->SetName("Cilinder");
		mesh->InitAsCilinder();
		break;
	}

	meshComponent->SetMesh(mesh);
	object->AddComponent(meshComponent);
	gameObjects.push_back(object);
	object->transform.SetEscale(10.f, 10.f, 10.f);
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


GameObject* M_Scene::GetGameObject(int uid)
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

			if (node->GetUuid() == uid)
				return node;


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

	return nullptr;
}


void M_Scene::GetGameObjectVector(std::vector<GameObject*>& vec)
{
	vec = gameObjects;
}


void M_Scene::DrawGameObjects(bool black)
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
				DrawObject(node, black);
			

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


void M_Scene::DrawObject(GameObject* object, bool blackWireframe)
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

	if (blackWireframe)
		color = &Black;
	
	mesh->Draw(object->transform.GetMatTransform().ptr(), texId, color, blackWireframe);
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
				{
					stack.push(node->childs[j]);
				}
			}
		}
	}
}


void M_Scene::DeleteAllGameObjects()
{
	int gameObjCount = gameObjects.size();
	for (int i = 0; i < gameObjCount; i++)
	{
		delete gameObjects[i];
		gameObjects[i] = nullptr;
	}

	gameObjects.clear();
}