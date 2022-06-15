#include "C_Portal.h"

#include "Config.h"

#include "GameObject.h"

#include "Application.h"
#include "M_Resources.h"
#include "M_Renderer3D.h"
#include "M_Camera3D.h"
#include "M_Scene.h"

#include "R_Mesh.h"
#include "R_Model.h"

C_Portal::C_Portal() : Component(COMPONENT_TYPE::PORTAL),
	aabb(),
	planeMeshId(0),

	connectedGO(0),
	debugDraw(true),
	debugDrawColor(1.0, 0.0, 0.0)
{
	//Request cube resource
	Resource* res = App->resourceManager->GetResourceByPath("/Assets/defaultAssets/primitives/Plane.fbx");
	if (res != nullptr) //Add reference to new resource
	{
		R_Model* model = static_cast<R_Model*>(res);

		planeMeshId = model->GetFirstMesh();

		R_Mesh* mesh = static_cast<R_Mesh*>(App->resourceManager->RequestResource(planeMeshId));

		if (mesh != nullptr)
		{
			mesh->AddReference();

			if (owner != nullptr)
				owner->transform.NotifyNeedUpdate();
		}
		else
			planeMeshId = 0;
	}
	else
		planeMeshId = 0;
}


C_Portal::~C_Portal()
{
	App->renderer3D->PopPortals();
	App->camera->PopPortals();

	if (connectedGO != 0)
	{
		GameObject* prevConnection = App->scene->GetGameObject(connectedGO);

		if (prevConnection != nullptr)
		{
			C_Portal* prevPortal = static_cast<C_Portal*>(prevConnection->GetComponent(COMPONENT_TYPE::PORTAL));
			if (prevPortal != nullptr)
				prevPortal->Disconnect();
		}
	}

	if (planeMeshId != 0)
	{
		Resource* res = App->resourceManager->RequestResource(planeMeshId);
		if (res != nullptr)
		{
			R_Mesh* mesh = (R_Mesh*)res;
			mesh->QuitReference();
		}
	}
	planeMeshId = 0;
}


bool C_Portal::PreUpdate(float dt)
{
	App->renderer3D->PushPortal(this);
	App->camera->PushPortal(this);

	return true;
}


void C_Portal::OnUpdateTransform(float4x4& transform)
{
	if (planeMeshId != 0)
	{
		Resource* res = App->resourceManager->RequestResource(planeMeshId);
		if (res != nullptr)
		{
			R_Mesh* mesh = (R_Mesh*)res;

			OBB obb = mesh->GetAABB();
			obb.Transform(transform);

			aabb.SetNegativeInfinity();
			aabb.Enclose(obb);
		}
	}
}


bool C_Portal::Connect(int uid)
{
	GameObject* go = App->scene->GetGameObject(uid);

	C_Portal* portal = static_cast<C_Portal*>(go->GetComponent(COMPONENT_TYPE::PORTAL));

	if (portal != nullptr && uid != owner->GetUuid() && uid != connectedGO)
	{
		if (connectedGO != 0)
		{
			GameObject* prevConnection = App->scene->GetGameObject(connectedGO);

			if (prevConnection != nullptr)
			{
				C_Portal* prevPortal = static_cast<C_Portal*>(prevConnection->GetComponent(COMPONENT_TYPE::PORTAL));
				if (prevPortal != nullptr)
					prevPortal->Disconnect();
			}
		}

		connectedGO = uid;
		portal->Connect(owner->GetUuid());
		return true;
	}

	else
	{
		return false;
	}
}


void C_Portal::Disconnect()
{
	connectedGO = 0;
}


int C_Portal::GetConnection() const
{
	return connectedGO;
}


bool C_Portal::CheckRayIntersection(LineSegment& ray)
{
	LineSegment auxRay = ray;
	float4x4 transform = GetOwner()->transform.GetMatTransform().Transposed().Inverted();
	auxRay.Transform(transform);

	if (planeMeshId != 0)
	{
		Resource* res = App->resourceManager->RequestResource(planeMeshId);
		if (res != nullptr)
		{
			R_Mesh* mesh = (R_Mesh*)res;
			return mesh->TestTriangleRayCollision(auxRay);
		}
	}

	return false;
}


bool C_Portal::GetDebugDraw() const
{
	return debugDraw;
}


void C_Portal::SetDebugDraw(bool draw)
{
	debugDraw = draw;
}


float* C_Portal::GetDebugDrawColor()
{
	return &debugDrawColor.x;
}


unsigned int C_Portal::GetVAO() const
{
	if (planeMeshId != 0)
	{
		Resource* res = App->resourceManager->RequestResource(planeMeshId);
		if (res != nullptr)
		{
			R_Mesh* mesh = (R_Mesh*)res;
			return mesh->GetVAO();
		}
	}

	return 0;
}


void C_Portal::GetVertexArray(std::vector<float>& vertexArray) const
{
	if (planeMeshId != 0)
	{
		Resource* res = App->resourceManager->RequestResource(planeMeshId);
		if (res != nullptr)
		{
			R_Mesh* mesh = (R_Mesh*)res;
			vertexArray = mesh->GetVertices();
		}
	}
}


void C_Portal::GetAllVertexData(std::vector<float>& vertexArray, std::vector<float>& normalsArray, std::vector<float>& texCoordArray, std::vector<unsigned int>& indicesArray) const
{
	if (planeMeshId != 0)
	{
		Resource* res = App->resourceManager->RequestResource(planeMeshId);
		if (res != nullptr)
		{
			R_Mesh* mesh = (R_Mesh*)res;
			mesh->GetAllVertexData(vertexArray, normalsArray, texCoordArray, indicesArray);
		}
	}
}


void C_Portal::GetAllVectorsSize(unsigned int& vertexCount, unsigned int& normalsCount, unsigned int& indexCount) const
{
	if (planeMeshId != 0)
	{
		Resource* res = App->resourceManager->RequestResource(planeMeshId);
		if (res != nullptr)
		{
			R_Mesh* mesh = (R_Mesh*)res;
			mesh->GetAllVectorsSize(vertexCount, normalsCount, indexCount);
		}
	}
}


unsigned int C_Portal::GetIndicesSize() const
{
	if (planeMeshId != 0)
	{
		Resource* res = App->resourceManager->RequestResource(planeMeshId);
		if (res != nullptr)
		{
			R_Mesh* mesh = (R_Mesh*)res;
			return mesh->GetIndicesSize();
		}
	}

	return 0;
}


unsigned int C_Portal::GetIndexOffset() const
{
	if (planeMeshId != 0)
	{
		Resource* res = App->resourceManager->RequestResource(planeMeshId);
		if (res != nullptr)
		{
			R_Mesh* mesh = (R_Mesh*)res;
			return mesh->GetIndicesOffset();
		}
	}

	return 0;
}


unsigned int C_Portal::GetVertexOffset() const
{
	if (planeMeshId != 0)
	{
		Resource* res = App->resourceManager->RequestResource(planeMeshId);
		if (res != nullptr)
		{
			R_Mesh* mesh = (R_Mesh*)res;
			return mesh->GetVertexOffset();
		}
	}

	return 0;
}


const float* C_Portal::GetAABBMinPoint() const
{
	return aabb.minPoint.ptr();
}


const float* C_Portal::GetAABBMaxPoint() const
{
	return aabb.maxPoint.ptr();
}


void C_Portal::Load(Config& node)
{
	/*cubeMeshId = node.GetNum("mesh");			//Technically, i can make it generic enought so that any mesh can be used, which would be cool

	Resource* res = App->resourceManager->RequestResource(cubeMeshId);
	if (res != nullptr)
	{
		R_Mesh* mesh = (R_Mesh*)res;
		mesh->AddReference();
	}
	else
		meshId = 0;*/

	connectedGO = node.GetNum("connectedGO");

	debugDraw = node.GetBool("debugDraw");
	debugDrawColor.x = node.GetNum("debugDrawColR");
	debugDrawColor.y = node.GetNum("debugDrawColG");
	debugDrawColor.z = node.GetNum("debugDrawColB");
}


void C_Portal::Save(Config& node) const
{
	node.AppendNum("type", (int)COMPONENT_TYPE::PORTAL);

	node.AppendNum("connectedGO", connectedGO);

	node.AppendBool("debugDraw", debugDraw);
	node.AppendNum("debugDrawColR", debugDrawColor.x);
	node.AppendNum("debugDrawColG", debugDrawColor.y);
	node.AppendNum("debugDrawColB", debugDrawColor.z);
}