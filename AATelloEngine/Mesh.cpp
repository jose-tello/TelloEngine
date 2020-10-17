#include "Mesh.h"
#include "MeshLoader.h"

#include "Glew/include/glew.h"
#pragma comment(lib,"Glew/libx86/glew32.lib")

#include <gl/GL.h>
#include <gl/GLU.h>

MeshEntry::MeshEntry() :
	vertexId(0),
	indexId(0),
	indexArrSize()
{
}


MeshEntry::~MeshEntry()
{
	glDeleteBuffers(1, &vertexId);
	glDeleteBuffers(1, &indexId);

	vertexId = 0;
	indexId = 0;
}


void MeshEntry::Init(float* vertexBuffer, std::size_t vertexArrSize, unsigned int* indexBuffer, std::size_t indexArrSize)
{
	this->indexArrSize = indexArrSize;

	GenVertexBuffer(vertexBuffer, vertexArrSize);
	GenIndexBuffer(indexBuffer);
}


void MeshEntry::Draw() const
{
	int idSize = indexArrSize / sizeof(unsigned int);

	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, vertexId);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexId);
	glDrawElements(GL_TRIANGLES, idSize, GL_UNSIGNED_INT, NULL);
	glDisableClientState(GL_VERTEX_ARRAY);
}


void MeshEntry::GenVertexBuffer(float* vertexBuffer, std::size_t vertexArrSize)
{
	glGenBuffers(1, (GLuint*)&(vertexId));
	glBindBuffer(GL_ARRAY_BUFFER, vertexId);
	glBufferData(GL_ARRAY_BUFFER, vertexArrSize, vertexBuffer, GL_STATIC_DRAW);
}


void MeshEntry::GenIndexBuffer(unsigned int* indexBuffer)
{
	glGenBuffers(1, (GLuint*)&(indexId));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexArrSize, indexBuffer, GL_STATIC_DRAW);
}



Mesh::Mesh(std::string& filename) :
	color(Red),
	transform()
{
	MeshLoader::Load(filename, meshEntryVector);
}


Mesh::Mesh(float* vertexBuffer, std::size_t vertexArrSize, unsigned int* indexBuffer, std::size_t indexArrSize, vec3& position, float angle, vec3& rotation, float red,
	float green, float blue, float alpha) :
	
	transform(),
	color(red, green, blue, alpha)
{
	meshEntryVector.push_back(MeshEntry());

	int meshEntryCount = meshEntryVector.size();

	for (int i = 0; i < meshEntryCount; i++)
	{
		meshEntryVector[i].Init(vertexBuffer, vertexArrSize, indexBuffer, indexArrSize);
	}

	SetPosition(position);
	SetRotation(angle, rotation);
}


Mesh::~Mesh()
{
	meshEntryVector.clear();
}


void Mesh::SetPosition(vec3& pos)
{
	transform.translate(pos.x, pos.y, pos.z);
}


void Mesh::SetRotation(float angle, vec3& rotation)
{
	transform.rotate(angle, rotation);
}


void Mesh::SetEscale(vec3& escale)
{
	transform.scale(escale.x, escale.y, escale.z);
}


void Mesh::Draw() const
{
	glPushMatrix();
	glMultMatrixf(transform.M);
	glColor3f(color.r, color.g, color.b);

	int meshEntryCount = meshEntryVector.size();

	for (int i = 0; i < meshEntryCount; i++)
	{
		meshEntryVector[i].Draw();
	}

	glPopMatrix();
}