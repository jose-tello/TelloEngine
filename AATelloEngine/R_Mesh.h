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

	float TestTriangleRayCollision(LineSegment& ray) const;

	void InitVertexBuffer(float*, size_t);
	void InitNormalBuffer(float*, size_t);
	void InitTexCoordBuffer(float*, size_t);
	void InitIndexBuffer(unsigned int*, size_t);

	void Draw(float* transformMatrix, unsigned int textureId, float* color, bool wireFrameBlack, bool drawVertexNormals, bool drawFaceNormals) const;

	void GetAllVertexData(std::vector<float>& vertexArray, std::vector<float>& normalsArray, std::vector<float>& texCoordArray, std::vector<unsigned int>& indicesArray) const;
	void GetAllVectorsSize(unsigned int& vert, unsigned int& norm, unsigned int& ind) const;

private:
	void InitAABB();

	void DrawVertexNormals() const;
	void DrawFaceNormals() const;

private:
	unsigned int vertexId;
	unsigned int normalsId;
	unsigned int texCoordId;
	unsigned int indexId;

	std::vector<float> vertices;
	std::vector<float> normals;
	std::vector<float> texCoords;
	std::vector<unsigned int> indices;

	int indexArrSize;

	AABB aabb;

};
#endif // !__R_MESH_H__
