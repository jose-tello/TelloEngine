#ifndef __MESH_H__
#define __MESH_H__

#include "Application.h"
#include "glmath.h"

#include "Color.h"

#include <string>

struct MeshEntry
{
public:
	MeshEntry();
	~MeshEntry();

	void Init(float*, std::size_t, unsigned int*, std::size_t);
	void Draw() const;

private:
	void GenVertexBuffer(float*, std::size_t);
	void GenIndexBuffer(unsigned int*);

private:
	unsigned int vertexId;
	unsigned int indexId;

	int indexArrSize;
};


struct Mesh
{
public:
	Mesh(std::string& filename);
	//Debug for primitives
	Mesh(float*, std::size_t, unsigned int*, std::size_t, vec3& position, float angle, vec3& rotation, float red = 0.f, float green = 0.f, float blue = 1.f, float alpha = 1.f);
	~Mesh();

	void SetPosition(vec3&);
	void SetRotation(float angle, vec3&);
	void SetEscale(vec3&);

	void Draw() const;	

private:
	std::vector<MeshEntry> meshEntryVector;

	mat4x4 transform;
	Color color;
	
};
#endif // !__MESH_H__
