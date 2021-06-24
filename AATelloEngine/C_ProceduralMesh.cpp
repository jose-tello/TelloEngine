#include "C_ProceduralMesh.h"

#include "Application.h"
#include "M_Editor.h"
#include "M_Renderer3D.h"

#include "Config.h"

#include "Glew/include/glew.h"
#pragma comment(lib,"Glew/libx86/glew32.lib")

#include <gl/GL.h>

const int VERTICES_PER_SQUARE = 3 * 2;
const int VERTEX_SIZE_BYTES = 2 * sizeof(float) + 4 * sizeof(float); //2 floats (pos) + 4 indicators
const int VERTEX_ATRIBUTE_DATA = 6;

C_ProceduralMesh::C_ProceduralMesh() : Component(COMPONENT_TYPE::PROCEDURAL_MESH),
	VAO(0u),
	rows(0u),
	columns(0u),
	aabb()
{
}


C_ProceduralMesh::~C_ProceduralMesh()
{
	glDeleteBuffers(1, &VAO);

	VAO = 0;

	vertices.clear();
}


void C_ProceduralMesh::OnUpdateTransform(float4x4& transform)
{
	
}


void C_ProceduralMesh::RecalculateMesh()
{
	rows = inspectorRows;
	columns = inspectorColumns;

	int gridCount = rows * columns;

	if (gridCount == 0)
	{
		App->editor->AddLog("[WARNING] Procedural mesh with 0 grids");
		return;
	}
	int totalVertexCount = gridCount * gridCount * VERTICES_PER_SQUARE;

	std::vector<float> buffer;
	buffer.reserve(totalVertexCount * VERTEX_ATRIBUTE_DATA);

	for (int i = 0; i < rows; ++i)
	{
		for (int j = 0; j < columns; ++j)
		{
			StoreGridScuare(j, i, buffer);
		}
	}
}


int C_ProceduralMesh::GetRows() const
{
	return (int)rows;
}


int C_ProceduralMesh::GetColumns() const
{
	return (int)columns;
}


AABB C_ProceduralMesh::GetAABB() const
{
	return aabb;
}


bool C_ProceduralMesh::TestAABBRayCollision(LineSegment& ray, float& distance) const
{
	return true;
}


float C_ProceduralMesh::TestTriangleCollision(LineSegment&, float4x4& transform) const
{
	return 100000.0f;
}


void C_ProceduralMesh::Load(Config& node)
{
	rows = node.GetNum("rows");
	columns = node.GetNum("columns");

	RecalculateMesh();
}


void C_ProceduralMesh::Save(Config& node) const
{
	node.AppendNum("rows", rows);
	node.AppendNum("columns", columns);
}

void C_ProceduralMesh::DrawAABB() const
{
	float3 corners[8];
	aabb.GetCornerPoints(corners);

	App->renderer3D->DrawCube((float*)corners, 0.1f, 1.0f, 0.2f);
}

void C_ProceduralMesh::StoreGridScuare(int col, int row, std::vector<float>& buffer)
{
	/*std::vector<float[2]> cornerPositions;
	float aux[2];

	aux[0] = col;
	aux[1] = row;
	cornerPositions.push_back(aux);

	aux[0] = col;
	aux[1] = row + 1;
	cornerPositions.push_back(aux);

	aux[0] = col + 1;
	aux[1] = row;
	cornerPositions.push_back(aux);

	aux[0] = col + 1;
	aux[1] = row + 1;
	cornerPositions.push_back(aux);*/
}