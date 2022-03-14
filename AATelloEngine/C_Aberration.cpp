#include "C_Aberration.h"

#include "Config.h"

#include "Application.h"
#include "M_Resources.h"
#include "R_Mesh.h"

C_Aberration::C_Aberration() : Component(COMPONENT_TYPE::ABERRATION),
	aabb(),
	cubeMeshId(0),

	deformationX(0.0),
	deformationY(0.0),
	deformationZ(0.0),

	debugDraw(true)
{
	//Request cube resource
}


C_Aberration::~C_Aberration()
{
	if (cubeMeshId != 0)
	{
		Resource* res = App->resourceManager->RequestResource(cubeMeshId);
		if (res != nullptr)
		{
			R_Mesh* mesh = (R_Mesh*)res;
			mesh->QuitReference();
		}
	}
	cubeMeshId = 0;
}

void C_Aberration::OnUpdateTransform(float4x4& transform)
{
	if (cubeMeshId != 0)
	{
		Resource* res = App->resourceManager->RequestResource(cubeMeshId);
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


float C_Aberration::GetDeformationX() const
{
	return deformationX;
}


float C_Aberration::GetDeformationY() const
{
	return deformationY;
}


float C_Aberration::GetDeformationZ() const
{
	return deformationZ;
}


void C_Aberration::SetDeformationX(float defX)
{
	deformationX = defX;
}


void C_Aberration::SetDeformationY(float defY)
{
	deformationY = defY;
}


void C_Aberration::SetDeformationZ(float defZ)
{
	deformationZ = defZ;
}


bool C_Aberration::GetDebugDraw() const
{
	return debugDraw;
}


void C_Aberration::SetDebugDraw(bool draw)
{
	debugDraw = draw;
}


void C_Aberration::GetAllVectorsSize(unsigned int& vertexCount, unsigned int& normalsCount, unsigned int& indexCount) const
{
	if (cubeMeshId != 0)
	{
		Resource* res = App->resourceManager->RequestResource(cubeMeshId);
		if (res != nullptr)
		{
			R_Mesh* mesh = (R_Mesh*)res;
			mesh->GetAllVectorsSize(vertexCount, normalsCount, indexCount);
		}
	}
}


unsigned int C_Aberration::GetIndicesSize() const
{
	if (cubeMeshId != 0)
	{
		Resource* res = App->resourceManager->RequestResource(cubeMeshId);
		if (res != nullptr)
		{
			R_Mesh* mesh = (R_Mesh*)res;
			return mesh->GetIndicesSize();
		}
	}

	return 0;
}


unsigned int C_Aberration::GetIndexOffset() const
{
	if (cubeMeshId != 0)
	{
		Resource* res = App->resourceManager->RequestResource(cubeMeshId);
		if (res != nullptr)
		{
			R_Mesh* mesh = (R_Mesh*)res;
			return mesh->GetIndicesOffset();
		}
	}

	return 0;
}


unsigned int C_Aberration::GetVertexOffset() const
{
	if (cubeMeshId != 0)
	{
		Resource* res = App->resourceManager->RequestResource(cubeMeshId);
		if (res != nullptr)
		{
			R_Mesh* mesh = (R_Mesh*)res;
			return mesh->GetVertexOffset();
		}
	}

	return 0;
}


const float* C_Aberration::GetAABBMinPoint() const
{
	return aabb.minPoint.ptr();
}


const float* C_Aberration::GetAABBMaxPoint() const
{
	return aabb.maxPoint.ptr();
}


void C_Aberration::Load(Config& node)
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

	deformationX = node.GetNum("deformationX");
	deformationY = node.GetNum("deformationY");
	deformationZ = node.GetNum("deformationZ");

	debugDraw = node.GetNum("debugDraw");
}


void C_Aberration::Save(Config& node) const
{
	node.AppendNum("type", (int)COMPONENT_TYPE::ABERRATION);

	//node.AppendNum("mesh", cubeMeshId);

	node.AppendNum("deformationX", deformationX);
	node.AppendNum("deformationY", deformationY);
	node.AppendNum("deformationZ", deformationZ);
	
	node.AppendBool("debugDraw", debugDraw);
}