#include "C_Mesh.h"
#include "Color.h"

#include "Config.h"

#include "GameObject.h"

#include "Application.h"
#include "M_Renderer3D.h"
#include "M_Resources.h"

#include "R_Mesh.h"


C_Mesh::C_Mesh() : Component(COMPONENT_TYPE::MESH),
	
	drawVertexNormals(false),
	drawFaceNormals(false),

	meshId(0),
	aabb()
{
}


C_Mesh::~C_Mesh()
{
	if (meshId != 0)
	{
		Resource* res = App->resourceManager->RequestResource(meshId);
		if (res != nullptr)
		{
			R_Mesh* mesh = (R_Mesh*)res;
			mesh->QuitReference();
		}
	}
	meshId = 0;
}


void C_Mesh::OnUpdateTransform(float4x4& transform)
{
	if (meshId != 0)
	{
		Resource* res = App->resourceManager->RequestResource(meshId);
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


void C_Mesh::SetMesh(int newMesh)
{
	if (meshId != 0)	//Quit reference to current resource
	{
		Resource* res = App->resourceManager->RequestResource(meshId);
		if (res != nullptr)
		{
			R_Mesh* mesh = (R_Mesh*)res;
			mesh->QuitReference();
		}
		else
			meshId = 0;
	}
		
	meshId = newMesh;

	Resource* res = App->resourceManager->RequestResource(meshId);

	if (res != nullptr) //Add reference to new resource
	{
		R_Mesh* mesh = (R_Mesh*)res;
		mesh->AddReference();		

		if (owner != nullptr)
			owner->transform.NotifyNeedUpdate();
	}
	else
		meshId = 0;
}


std::vector<float>& C_Mesh::GetVertices()
{
	if (meshId != 0)
	{
		Resource* res = App->resourceManager->RequestResource(meshId);
		if (res != nullptr)
		{
			R_Mesh* mesh = (R_Mesh*)res;
			return mesh->GetVertices();
		}
	}
	else
		assert("Dont have mesh!");
}


std::vector<unsigned int>& C_Mesh::GetIndices()
{
	if (meshId != 0)
	{
		Resource* res = App->resourceManager->RequestResource(meshId);
		if (res != nullptr)
		{
			R_Mesh* mesh = (R_Mesh*)res;
			return mesh->GetIndices();
		}
	}
	else
		assert("Dont have mesh!");
}


void C_Mesh::GetAllVectorsSize(unsigned int& vert, unsigned int& norm, unsigned int& ind) const
{
	if (meshId != 0)
	{
		Resource* res = App->resourceManager->RequestResource(meshId);
		if (res != nullptr)
		{
			R_Mesh* mesh = (R_Mesh*)res;
			mesh->GetAllVectorsSize(vert, norm, ind);
		}
	}
}


unsigned int C_Mesh::GetVAO() const
{
	if (meshId != 0)
	{
		Resource* res = App->resourceManager->RequestResource(meshId);
		if (res != nullptr)
		{
			R_Mesh* mesh = (R_Mesh*)res;
			return mesh->GetVAO();
		}
	}

	return 0;
}


unsigned int C_Mesh::GetIndicesSize() const
{
	if (meshId != 0)
	{
		Resource* res = App->resourceManager->RequestResource(meshId);
		if (res != nullptr)
		{
			R_Mesh* mesh = (R_Mesh*)res;
			return mesh->GetIndicesSize();
		}
	}

	return 0;
}


unsigned int C_Mesh::GetIndexOffset() const
{
	if (meshId != 0)
	{
		Resource* res = App->resourceManager->RequestResource(meshId);
		if (res != nullptr)
		{
			R_Mesh* mesh = (R_Mesh*)res;
			return mesh->GetIndicesOffset();
		}
	}

	return 0;
}


unsigned int C_Mesh::GetVertexOffset() const
{
	if (meshId != 0)
	{
		Resource* res = App->resourceManager->RequestResource(meshId);
		if (res != nullptr)
		{
			R_Mesh* mesh = (R_Mesh*)res;
			return mesh->GetVertexOffset();
		}
	}

	return 0;
}


AABB C_Mesh::GetAABB() const
{
	return aabb;
}


const float* C_Mesh::GetAABBMinPoint() const
{
	return aabb.minPoint.ptr();
}


const float* C_Mesh::GetAABBMaxPoint() const
{
	return aabb.maxPoint.ptr();
}


bool C_Mesh::TestAABBRayCollision(LineSegment& ray, float& distance) const
{
	float farDst;
	return ray.Intersects(aabb, distance, farDst);
}


float C_Mesh::TestTriangleCollision(LineSegment& ray, float4x4& transform) const
{
	if (meshId != 0)
	{
		LineSegment auxRay = ray;
		auxRay.Transform(transform.Transposed().Inverted());

		Resource* res = App->resourceManager->RequestResource(meshId);
		if (res != nullptr)
		{
			R_Mesh* mesh = (R_Mesh*)res;
			return mesh->TestTriangleRayCollision(auxRay);
		}
		else
			return 0;
	}
	else
		return 0;
}


//Handles aabb, vertex normals and face normals draw calls
void C_Mesh::HandleDebugDraws() const
{
	Resource* res = App->resourceManager->RequestResource(meshId);
	if (res != nullptr)
	{
		R_Mesh* mesh = (R_Mesh*)res;
		
		if (drawVertexNormals == true)
			mesh->DrawVertexNormals();

		if (drawFaceNormals == true)
			mesh->DrawFaceNormals();
	}
}


void C_Mesh::Load(Config& node)
{
	meshId = node.GetNum("mesh");

	Resource* res = App->resourceManager->RequestResource(meshId);
	if (res != nullptr)
	{
		R_Mesh* mesh = (R_Mesh*)res;
		mesh->AddReference();
	}
	else
		meshId = 0;
}


void C_Mesh::Save(Config& node) const
{
	node.AppendNum("type", (int)COMPONENT_TYPE::MESH);

	node.AppendNum("mesh", meshId);
}


void C_Mesh::DrawAABB() const
{
	float3 corners[8];
	aabb.GetCornerPoints(corners);

	App->renderer3D->DrawCube((float*)corners, 0.1f, 1.0f, 0.2f);
}