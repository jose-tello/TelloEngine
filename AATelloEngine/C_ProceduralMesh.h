#ifndef __C_PROCEDURAL_MESH_H__
#define __C_PROCEDURAL_MESH_H__

#include "Component.h"
#include "MathGeoLib/src/MathGeoLib.h"

#include <vector>

class C_ProceduralMesh : public Component
{
public:
	C_ProceduralMesh();
	~C_ProceduralMesh() override;

	void OnUpdateTransform(float4x4& transform) override;	//TODO: need to complete

	void RecalculateMesh();

	int GetRows() const;
	int GetColumns() const;

	AABB GetAABB() const;	//TODO: Return a reference
	bool TestAABBRayCollision(LineSegment& ray, float& distance) const;	//TODO: need to complete
	float TestTriangleCollision(LineSegment&, float4x4& transform) const;	//TODO: need to complete

	void Load(Config&) override;
	void Save(Config&) const override;

	void DrawAABB() const;

private:
	void StoreGridScuare(int col, int row, std::vector<float>& buffer);
	
public:
	int inspectorRows = 0u;
	int inspectorColumns = 0u;

private:
	unsigned int VAO = 0u;

	std::vector<float> vertices;

	//Counter of rows and columns
	unsigned int rows = 0u;
	unsigned int columns = 0u;

	AABB aabb;
};

#endif // !__C_PROCEDURAL_MESH_H__
