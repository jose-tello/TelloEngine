#ifndef __C_MESH_H__
#define __C_MESH_H__

#include "Component.h"
#include "MathGeoLib/src/MathGeoLib.h"

#include <vector>

class C_Mesh : public Component
{
public:
	C_Mesh();
	~C_Mesh() override;

	void OnUpdateTransform(float4x4& transform) override;

	void Draw(float* transformMatrix, unsigned int textureId, float* col, bool wireframeMode) const;

	void SetMesh(int meshId);

	void GetAllVectorsSize(unsigned int&, unsigned int&, unsigned int&) const;
	unsigned int GetVAO() const;
	unsigned int GetIndicesSize() const;

	AABB GetAABB() const;
	bool TestAABBRayCollision(LineSegment& ray, float& distance) const;
	float TestTriangleCollision(LineSegment&, float4x4& transform) const;

	//Handles aabb, vertex normals and face normals draw calls
	void HandleDebugDraws(bool drawAABB) const;

	void Load(Config&) override;
	void Save(Config&) const override;

	void DrawAABB() const;

public:
	bool drawVertexNormals;
	bool drawFaceNormals;

private:
	int meshId;
	AABB aabb;
};


#endif // !__C_MESH_H__

