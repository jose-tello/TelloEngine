#include "Mesh.h"
#include "MeshLoader.h"

#include "Glew/include/glew.h"
#pragma comment(lib,"Glew/libx86/glew32.lib")

#include <gl/GL.h>
#include <gl/GLU.h>

MeshEntry::MeshEntry() :
	vertexId(0),
	normalsId(0),
	texCoordId(0),
	indexId(0),
	indexArrSize()
{
}


MeshEntry::~MeshEntry()
{
	glDeleteBuffers(1, &vertexId);
	glDeleteBuffers(1, &normalsId);
	glDeleteBuffers(1, &texCoordId);
	glDeleteBuffers(1, &indexId);

	vertexId = 0;
	normalsId = 0;
	texCoordId = 0;
	indexId = 0;
}


void MeshEntry::Init(float* vertexBuffer, std::size_t vertexArrSize, unsigned int* indexBuffer, std::size_t indexArrSize)
{
	InitVertexBuffer(vertexBuffer, vertexArrSize);
	InitIndexBuffer(indexBuffer, indexArrSize);
}


void MeshEntry::InitVertexBuffer(float* vertexBuffer, std::size_t vertexArrSize)
{
	vertices.resize(vertexArrSize / sizeof(float));
	memcpy(&vertices[0], vertexBuffer, vertexArrSize);

	glGenBuffers(1, (GLuint*)&(vertexId));
	glBindBuffer(GL_ARRAY_BUFFER, vertexId);
	glBufferData(GL_ARRAY_BUFFER, vertexArrSize, vertexBuffer, GL_STATIC_DRAW);
}


void MeshEntry::InitNormalBuffer(float* normalsBuffer, std::size_t normalsArrSize)
{
	normals.resize(normalsArrSize / sizeof(float));
	memcpy(&normals[0], normalsBuffer, normalsArrSize);

	glGenBuffers(1, (GLuint*)&(normalsId));
	glBindBuffer(GL_NORMAL_ARRAY, normalsId);
	glBufferData(GL_NORMAL_ARRAY, normalsArrSize, normalsBuffer, GL_STATIC_DRAW);
}


void MeshEntry::InitTexCoordBuffer(float* texBuffer, std::size_t texArrSize)
{
	glGenBuffers(1, (GLuint*)&(texCoordId));
	glBindBuffer(GL_TEXTURE_COORD_ARRAY, texCoordId);
	glBufferData(GL_TEXTURE_COORD_ARRAY, texArrSize, texBuffer, GL_TEXTURE_COORD_ARRAY);
}


void MeshEntry::InitIndexBuffer(unsigned int* indexBuffer, std::size_t indexArrSize)
{
	this->indexArrSize = indexArrSize;

	indices.resize(indexArrSize / sizeof(unsigned int));
	memcpy(&indices[0], indexBuffer, indexArrSize);

	glGenBuffers(1, (GLuint*)&(indexId));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexArrSize, indexBuffer, GL_STATIC_DRAW);
}


void MeshEntry::Draw() const
{
	int idSize = indexArrSize / sizeof(unsigned int);

	glEnableClientState(GL_VERTEX_ARRAY);

	if (normalsId != 0)
		glEnableClientState(GL_NORMAL_ARRAY);
	
	if (texCoordId != 0)
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, vertexId);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	if (normalsId != 0)
		glNormalPointer(GL_FLOAT, 0, NULL);
	
	if (texCoordId != 0)
		glTexCoordPointer(2, GL_FLOAT, 0, NULL);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexId);
	glDrawElements(GL_TRIANGLES, idSize, GL_UNSIGNED_INT, NULL);
	glDisableClientState(GL_VERTEX_ARRAY);	
}


void MeshEntry::DrawVertexNormals() const
{
	if (normalsId != 0)
	{
		glLineWidth(2.0f);
		glColor3f(0, 0, 1);

		glBegin(GL_LINES);

		for (int i = 0; i < vertices.size(); i += 3)
		{
			glVertex3f(vertices[i], vertices[i + 1], vertices[i + 2]);
			glVertex3f(vertices[i] + normals[i], vertices[i + 1] + normals[i + 1], vertices[i + 2] + normals[i + 2]);
		}

		glEnd();
	}
}


void MeshEntry::DrawFaceNormals() const
{
	glLineWidth(2.0f);
	glColor3f(0, 1, 0);

	glBegin(GL_LINES);

	for (int i = 0; i < indices.size(); i += 3)
	{
		unsigned int index0 = indices[i];
		unsigned int index1 = indices[i + 1];
		unsigned int index2 = indices[i + 2];

		vec3 P0(vertices[index0 * 3], vertices[index0 * 3 + 1], vertices[index0 * 3 + 2]);
		vec3 P1(vertices[index1 * 3], vertices[index1 * 3 + 1], vertices[index1 * 3 + 2]);
		vec3 P2(vertices[index2 * 3], vertices[index2 * 3 + 1], vertices[index2 * 3 + 2]);

		vec3 V0 = P0 - P1;
		vec3 V1 = P2 - P1;

		vec3 N = cross(V1, V0);
		N = normalize(N) * 3;

		vec3 center = (P0 + P1 + P2) / 3;

		glVertex3f(center.x, center.y, center.z);
		glVertex3f(center.x + N.x, center.y + N.y, center.z + N.z);
	}

	glEnd();
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


void Mesh::Draw(bool drawVertexNormals, bool drawFaceNormals) const
{
	glPushMatrix();
	glMultMatrixf(transform.M);
	glColor3f(color.r, color.g, color.b);

	int meshEntryCount = meshEntryVector.size();

	for (int i = 0; i < meshEntryCount; i++)
	{
		meshEntryVector[i].Draw();

		if (drawVertexNormals)
			meshEntryVector[i].DrawVertexNormals();
		
		if (drawFaceNormals)
			meshEntryVector[i].DrawFaceNormals();
	}

	glPopMatrix();
}