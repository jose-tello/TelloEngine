#include "C_Mesh.h"
#include "Color.h"

#include "Mesh.h"

#include "Config.h"
#include "MeshImporter.h"

#include "Application.h"
#include "M_FileManager.h"
#include "M_Renderer3D.h"

C_Mesh::C_Mesh() : Component(COMPONENT_TYPE::MESH),
	
	drawVertexNormals(false),
	drawFaceNormals(false),

	mesh(nullptr),
	aabb()
{
}


C_Mesh::C_Mesh(std::vector<float>& vertexBuff, std::vector<float>& normals, std::vector<float> texCoords, std::vector<unsigned int> indices) : Component(COMPONENT_TYPE::MESH),
	drawVertexNormals(false),
	drawFaceNormals(false),

	mesh(nullptr)
{
	mesh = new Mesh(vertexBuff, normals, texCoords, indices);
}


C_Mesh::~C_Mesh()
{
	delete mesh;
	mesh = nullptr;
}



void C_Mesh::OnUpdateTransform(float4x4& transform)
{
	OBB obb = mesh->GetAABB();
	obb.Transform(transform);

	aabb.SetNegativeInfinity();
	aabb.Enclose(obb);
}


void C_Mesh::Draw(float* transformMatrix, unsigned int textureId, float* color, bool wireFrameBlack) const
{
	mesh->Draw(transformMatrix, textureId, color, wireFrameBlack, drawVertexNormals, drawFaceNormals);
}


void C_Mesh::SetMesh(Mesh* mesh)
{
	if (this->mesh != nullptr)
		delete this->mesh;

	this->mesh = mesh;
}


Mesh* C_Mesh::GetMesh() const
{
	return mesh;
}


void C_Mesh::GetAllVectorsSize(unsigned int& vert, unsigned int& norm, unsigned int& ind) const
{
	mesh->GetAllVectorsSize(vert, norm, ind);
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
	ray.Transform(transform.Inverted());
	return mesh->TestTriangleRayCollision(ray);
}


void C_Mesh::DrawAABB() const
{
	float3 corners[8];
	aabb.GetCornerPoints(corners);

	App->renderer3D->DrawCube((float*)corners);
}


void C_Mesh::Load(Config& node)
{
	if (mesh == nullptr)
		mesh = new Mesh();
	
	const char* meshName = node.GetString("mesh");
	std::string meshPath(MESH_LIBRARY);
	meshPath.append(meshName);

	MeshImporter::Load(mesh, meshPath.c_str());
}


void C_Mesh::Save(Config& node) const
{
	node.AppendNum("type", (int)COMPONENT_TYPE::MESH);

	node.AppendString("mesh", mesh->meshPath.c_str());
}