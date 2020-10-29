#include "C_Mesh.h"
#include "Color.h"

#include "Primitive.h"

#include "Glew/include/glew.h"
#pragma comment(lib,"Glew/libx86/glew32.lib")

#include <gl/GL.h>
#include "glmath.h"


C_Mesh::C_Mesh() : Component(COMPONENT_TYPE::MESH),
	
	drawVertexNormals(false),
	drawFaceNormals(false),

	vertexId(0),
	normalsId(0),
	texCoordId(0),
	indexId(0),

	indexArrSize(0)
{
}


C_Mesh::~C_Mesh()
{
	glDeleteBuffers(1, &vertexId);
	glDeleteBuffers(1, &normalsId);
	glDeleteBuffers(1, &indexId);
	glDeleteBuffers(1, &texCoordId);

	vertexId = 0;
	normalsId = 0;
	indexId = 0;
	texCoordId = 0;

	vertices.clear();
	normals.clear();
	indices.clear();
}


void C_Mesh::InitAsCube()
{
	Primitive::CreateCube(vertices, indices);

	InitVertexBuffer(&vertices[0], vertices.size() * sizeof(float));
	InitIndexBuffer(&indices[0], indices.size() * sizeof(unsigned int));
}


void C_Mesh::InitVertexBuffer(float* vertexBuffer, std::size_t vertexArrSize)
{
	vertices.resize(vertexArrSize / sizeof(float));
	memcpy(&vertices[0], vertexBuffer, vertexArrSize);

	glGenBuffers(1, (GLuint*)&(vertexId));
	glBindBuffer(GL_ARRAY_BUFFER, vertexId);
	glBufferData(GL_ARRAY_BUFFER, vertexArrSize, vertexBuffer, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void C_Mesh::InitNormalBuffer(float* normalsBuffer, size_t normalsArrSize)
{
	normals.resize(normalsArrSize / sizeof(float));
	memcpy(&normals[0], normalsBuffer, normalsArrSize);

	glGenBuffers(1, (GLuint*)&(normalsId));
	glBindBuffer(GL_ARRAY_BUFFER, normalsId);
	glBufferData(GL_ARRAY_BUFFER, normalsArrSize, normalsBuffer, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void C_Mesh::InitTexCoordBuffer(float* texBuffer, std::size_t texArrSize)
{
	glGenBuffers(1, (GLuint*)&(texCoordId));
	glBindBuffer(GL_ARRAY_BUFFER, texCoordId);
	glBufferData(GL_ARRAY_BUFFER, texArrSize, texBuffer, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void C_Mesh::InitIndexBuffer(unsigned int* indexBuffer, std::size_t indexArrSize)
{
	this->indexArrSize = indexArrSize;

	indices.resize(indexArrSize / sizeof(unsigned int));
	memcpy(&indices[0], indexBuffer, indexArrSize);

	glGenBuffers(1, (GLuint*)&(indexId));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexArrSize, indexBuffer, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


void C_Mesh::Draw(float* transformMatrix, unsigned int textureId, float* color, bool wireFrameBlack) const
{
	glPushMatrix();
	glMultMatrixf(transformMatrix);

	if (color != nullptr)
		glColor3f(color[0], color[1], color[2]);
	else
		glColor3f(1.f, 1.f, 1.f);	//Reset the color

	int idSize = indexArrSize / sizeof(unsigned int);

	glEnableClientState(GL_VERTEX_ARRAY);

	if (normalsId != 0)
		glEnableClientState(GL_NORMAL_ARRAY);

	if (texCoordId != 0)
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);


	glBindBuffer(GL_ARRAY_BUFFER, vertexId);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	if (normalsId != 0)
	{
		glBindBuffer(GL_ARRAY_BUFFER, normalsId);
		glNormalPointer(GL_FLOAT, 0, NULL);
	}


	if (texCoordId != 0)
	{
		glBindBuffer(GL_ARRAY_BUFFER, texCoordId);
		glTexCoordPointer(2, GL_FLOAT, 0, NULL);

		if (textureId != 0)
		{
			glBindTexture(GL_TEXTURE_2D, textureId);
		}
	}


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexId);
	glDrawElements(GL_TRIANGLES, idSize, GL_UNSIGNED_INT, NULL);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	if (wireFrameBlack == false)
	{
		if (drawVertexNormals)
			DrawVertexNormals();

		if (drawFaceNormals == true)
			DrawFaceNormals();
	}

	glPopMatrix();
}


void C_Mesh::DrawVertexNormals() const
{
	if (normalsId != 0)
	{
		glLineWidth(3.0f);
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


void C_Mesh::DrawFaceNormals() const
{
	glLineWidth(3.0f);
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


void C_Mesh::GetAllVectorsSize(unsigned int& vert, unsigned int& norm, unsigned int& ind) const
{
	vert = vertices.size();
	norm = normals.size();
	ind = indices.size();
}