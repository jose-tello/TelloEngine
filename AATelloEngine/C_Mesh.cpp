#include "C_Mesh.h"
#include "Color.h"

#include "Config.h"
#include "MeshImporter.h"

#include "Application.h"
#include "M_FileManager.h"
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
		R_Mesh* mesh = (R_Mesh*)App->resourceManager->RequestResource(meshId);
		if (mesh != nullptr)
			mesh->QuitReference();
	}

	meshId = 0;
}



void C_Mesh::OnUpdateTransform(float4x4& transform)
{
	if (meshId != 0)
	{
		R_Mesh* mesh = (R_Mesh*)App->resourceManager->RequestResource(meshId);

		if (mesh != nullptr)
		{
			OBB obb = mesh->GetAABB();
			obb.Transform(transform);

			aabb.SetNegativeInfinity();
			aabb.Enclose(obb);
		}
		else
		{
			meshId = 0;
			aabb.SetNegativeInfinity();
		}
	}

	else
		aabb.SetNegativeInfinity();
}


void C_Mesh::Draw(float* transformMatrix, unsigned int textureId, float* color, bool wireFrameBlack) const
{
	if (meshId != 0)
	{
		R_Mesh* mesh = (R_Mesh*)App->resourceManager->RequestResource(meshId);

		if (mesh != nullptr)
			mesh->Draw(transformMatrix, textureId, color, wireFrameBlack, drawVertexNormals, drawFaceNormals);
	}
}


void C_Mesh::SetMesh(int newMesh)
{
	if (meshId != 0)
	{
		R_Mesh* mesh = (R_Mesh*)App->resourceManager->RequestResource(meshId);
		if (mesh != nullptr)
			mesh->QuitReference();
	}
		
	meshId = newMesh;

	R_Mesh* mesh = (R_Mesh*)App->resourceManager->RequestResource(newMesh);
	mesh->AddReference();
}


void C_Mesh::GetAllVectorsSize(unsigned int& vert, unsigned int& norm, unsigned int& ind) const
{
	if (meshId != 0)
	{
		R_Mesh* mesh = (R_Mesh*)App->resourceManager->RequestResource(meshId);

		if (mesh != nullptr)
			mesh->GetAllVectorsSize(vert, norm, ind);
	}
}


AABB C_Mesh::GetAABB() const
{
	return aabb;
}


bool C_Mesh::TestAABBRayCollision(LineSegment& ray, float& distance) const
{
	float farDst;
	return ray.Intersects(aabb, distance, farDst);
}


float C_Mesh::TestTriangleCollision(LineSegment ray, float4x4& transform) const
{
	if (meshId != 0)
	{
		ray.Transform(transform.Inverted());

		R_Mesh* mesh = (R_Mesh*)App->resourceManager->RequestResource(meshId);

		if (mesh != nullptr)
			return mesh->TestTriangleRayCollision(ray);
	}
	else
		return false;
}


void C_Mesh::DrawAABB() const
{
	float3 corners[8];
	aabb.GetCornerPoints(corners);

	App->renderer3D->DrawCube((float*)corners);
}


void C_Mesh::Load(Config& node)
{
	meshId = node.GetNum("mesh");

	R_Mesh* mesh = (R_Mesh*)App->resourceManager->RequestResource(meshId);
	mesh->AddReference();
}


void C_Mesh::Save(Config& node) const
{
	node.AppendNum("type", (int)COMPONENT_TYPE::MESH);

	node.AppendNum("mesh", meshId);
}