#ifndef __MESH_H__
#define __MESH_H__

#include <vector>

struct Mesh
{
public:
	Mesh();
	Mesh(std::vector<float>& vertexBuff, std::vector<float>& normals, std::vector<float> texCoords, std::vector<unsigned int> indices);
	~Mesh();

	//Primitive things
	void InitAsCube();
	void InitAsPiramid();
	void InitAsSphere();
	void InitAsCilinder();

	void InitVertexBuffer(float*, size_t);
	void InitNormalBuffer(float*, size_t);
	void InitTexCoordBuffer(float*, size_t);
	void InitIndexBuffer(unsigned int*, size_t);

	void GetAllVectorsSize(unsigned int& vert, unsigned int& norm, unsigned int& ind) const;

	void Draw(float* transformMatrix, unsigned int textureId, float* color, bool wireFrameBlack, bool drawVertexNormals, bool drawFaceNormals) const;

private:
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
};
#endif // !__MESH_H__

