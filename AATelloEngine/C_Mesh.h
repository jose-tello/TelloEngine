#ifndef __C_MESH_H__
#define __C_MESH_H__

#include "Component.h"
#include "MathGeoLib/src/MathGeoLib.h"

#include <vector>

struct Mesh;

class C_Mesh : public Component
{
public:
	C_Mesh();
	C_Mesh(std::vector<float>& vertexBuff, std::vector<float>& normals, std::vector<float> texCoords, std::vector<unsigned int> indices);
	~C_Mesh() override;

	void OnUpdateTransform(float4x4& transform) override;

	void Draw(float* transformMatrix, unsigned int textureId, float* col, bool wireframeMode) const;

	void SetMesh(Mesh*);
	Mesh* GetMesh() const;
	void GetAllVectorsSize(unsigned int&, unsigned int&, unsigned int&) const;

	bool TestAABBRayCollision(LineSegment& ray, float& distance) const;
	float TestTriangleCollision(LineSegment, float4x4& transform);

	void Load(Config&) override;
	void Save(Config&) const override;

public:
	bool drawVertexNormals;
	bool drawFaceNormals;

private:
	Mesh* mesh = nullptr;
	//AABB aabb;
};


#endif // !__C_MESH_H__

