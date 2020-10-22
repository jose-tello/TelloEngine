#ifndef __MESH_H__
#define __MESH_H__

#include "Application.h"
#include "glmath.h"

#include "Color.h"

#include <string>

#define CHECKERS_WIDTH 100
#define CHECKERS_HEIGHT 100

enum class PRIMITIVE_TYPE : int
{
	NONE = -1,
	CUBE,
	PIRAMID,
	SPHERE,
	CILINDER
};

struct MeshEntry
{
public:
	MeshEntry();
	MeshEntry(const MeshEntry& copy);
	~MeshEntry();

	void InitAsCube();
	void InitAsPiramid();
	void InitAsSphere(float radius, unsigned int rings, unsigned int sectors);
	void InitAsCilinder(float radius, unsigned int sectors, float height);

	void InitVertexBuffer(float*, std::size_t);
	void InitNormalBuffer(float*, std::size_t);
	void InitTexCoordBuffer(float*, std::size_t);
	void InitIndexBuffer(unsigned int*, std::size_t);
	void InitTexture();

	void Draw() const;
	void DrawVertexNormals() const;
	void DrawFaceNormals() const;

private:
	unsigned int vertexId;
	unsigned int normalsId;
	unsigned int indexId;

	unsigned int texCoordId;
	unsigned int textureId;

	std::vector<float> vertices;
	std::vector<float> normals;
	std::vector<float> texCoords;
	std::vector<unsigned int> indices;

	int indexArrSize;
};


struct Mesh
{
public:
	Mesh(std::string& filename);
	Mesh(std::vector<MeshEntry>&);
	Mesh(const Mesh&);	//copy constructor

	//Debug for primitives
	Mesh(PRIMITIVE_TYPE, Color, float radius = 0, unsigned int rings = 0, unsigned int sectors = 0, float height = 0);
	~Mesh();

	void SetPosition(vec3&);
	void SetRotation(float angle, vec3&);
	void SetEscale(vec3&);

	void Draw(bool drawVertexNormals, bool drawFaceNormals, bool black = false) const;	

private:
	std::vector<MeshEntry> meshEntryVector;

	mat4x4 transform;
	Color color;
	
};
#endif // !__MESH_H__
