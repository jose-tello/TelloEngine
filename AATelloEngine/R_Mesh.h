#ifndef __R_MESH_H__
#define __R_MESH_H__

#include "Resource.h"

#include "MathGeoLib/src/Geometry/AABB.h"

#include <vector>

class R_Mesh : public Resource
{
public:
	R_Mesh(int uid, const char* path, RESOURCE_TYPE type);
	~R_Mesh() override;

	void Load() override;
	void UnLoad() override;

	AABB GetAABB() const;

	void InitVAO(float* vertices, size_t vertSize, unsigned int* indices, size_t indexSize, float* normals,
				 size_t normalsSize, float* texCoords, size_t texCoordsSize);

	float TestTriangleRayCollision(LineSegment& ray) const;

	unsigned int GetVAO() const;
	unsigned int GetIndicesSize() const;

	std::vector<float>& GetVertices();
	std::vector<unsigned int>& GetIndices();

	void GetAllVertexData(std::vector<float>& vertexArray, std::vector<float>& normalsArray, std::vector<float>& texCoordArray, std::vector<unsigned int>& indicesArray) const;
	void GetAllVectorsSize(unsigned int& vert, unsigned int& norm, unsigned int& ind) const;

	void DrawVertexNormals() const;
	void DrawFaceNormals() const;

	unsigned int GetIndicesOffset() const;
	void SetIndicesOffset(int offset);

private:
	void InitAABB();

	void InitArrayBuffer(float*, size_t, float*, size_t, float*, size_t);
	void InitIndexBuffer(unsigned int*, size_t);

private:
	unsigned int VAO = 0;
	int indicesOffset = 0;

	std::vector<float> vertices;
	std::vector<float> normals;
	std::vector<float> texCoords;
	std::vector<unsigned int> indices;
	std::vector<float> arrayBuffer;

	AABB aabb;

};
#endif // !__R_MESH_H__
