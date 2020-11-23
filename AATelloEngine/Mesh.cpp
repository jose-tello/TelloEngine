#include "Mesh.h"

#include "MathGeoLib/src/MathGeoLib.h"

#include "Glew/include/glew.h"
#pragma comment(lib,"Glew/libx86/glew32.lib")

#include <gl/GL.h>

#include "Primitive.h"

Mesh::Mesh() :
	meshPath(),
	vertexId(0),
	normalsId(0),
	texCoordId(0),
	indexId(0),

	indexArrSize(0),

	aabb()
{
}


Mesh::Mesh(std::vector<float>& vertexBuff, std::vector<float>& normals, std::vector<float> texCoords, std::vector<unsigned int> indices) :
	meshPath(),
	vertexId(0),
	normalsId(0),
	texCoordId(0),
	indexId(0),

	indexArrSize(0),

	aabb()
{
	InitVertexBuffer(&vertexBuff[0], vertexBuff.size() * sizeof(float));

	if (normals.empty() == false)
		InitNormalBuffer(&normals[0], normals.size() * sizeof(float));

	if (texCoords.empty() == false)
		InitTexCoordBuffer(&texCoords[0], texCoords.size() * sizeof(float));

	if (indices.empty() == false)
		InitIndexBuffer(&indices[0], indices.size() * sizeof(unsigned int));
}


Mesh::~Mesh()
{
	glDeleteBuffers(1, &vertexId);
	glDeleteBuffers(1, &normalsId);
	glDeleteBuffers(1, &texCoordId);
	glDeleteBuffers(1, &indexId);

	vertexId = 0;
	normalsId = 0;
	indexId = 0;
	texCoordId = 0;

	vertices.clear();
	normals.clear();
	texCoords.clear();
	indices.clear();
}


AABB Mesh::GetAABB() const
{
	return aabb;
}


void Mesh::SetAABB(float4x4& transform)
{
	aabb.SetNegativeInfinity();
	aabb.Enclose((float3*)&vertices[0], vertices.size() / 3);

	OBB obb = aabb;
	obb.Transform(transform);

	aabb.SetNegativeInfinity();
	aabb.Enclose(obb);
}


bool Mesh::TestAABBRayCollision(LineSegment& ray, float& distance) const
{
	float farDst;
	return ray.Intersects(aabb, distance, farDst);
}


//Primitive things
void Mesh::InitAsCube()
{
	Primitive::CreateCube(vertices, indices);

	InitVertexBuffer(&vertices[0], vertices.size() * sizeof(float));
	InitIndexBuffer(&indices[0], indices.size() * sizeof(unsigned int));
}


void Mesh::InitAsPiramid()
{
	Primitive::CreatePiramid(vertices, indices);

	InitVertexBuffer(&vertices[0], vertices.size() * sizeof(float));
	InitIndexBuffer(&indices[0], indices.size() * sizeof(unsigned int));
}


void Mesh::InitAsSphere()
{
	Primitive::CreateSphere(2.f, 20, 20, vertices, normals, texCoords, indices);

	InitVertexBuffer(&vertices[0], vertices.size() * sizeof(float));
	InitNormalBuffer(&normals[0], normals.size() * sizeof(float));
	InitTexCoordBuffer(&texCoords[0], texCoords.size() * sizeof(float));
	InitIndexBuffer(&indices[0], indices.size() * sizeof(unsigned int));
}


void Mesh::InitAsCilinder()
{
	Primitive::CreateCilinder(2.f, 20, 3, vertices, normals, texCoords, indices);

	InitVertexBuffer(&vertices[0], vertices.size() * sizeof(float));
	InitNormalBuffer(&normals[0], normals.size() * sizeof(float));
	InitTexCoordBuffer(&texCoords[0], texCoords.size() * sizeof(float));
	InitIndexBuffer(&indices[0], indices.size() * sizeof(unsigned int));
}


void Mesh::InitVertexBuffer(float* vertexBuffer, size_t vertexArrSize)
{
	vertices.resize(vertexArrSize / sizeof(float));
	memcpy(&vertices[0], vertexBuffer, vertexArrSize);

	glGenBuffers(1, (GLuint*)&(vertexId));
	glBindBuffer(GL_ARRAY_BUFFER, vertexId);
	glBufferData(GL_ARRAY_BUFFER, vertexArrSize, vertexBuffer, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void Mesh::InitNormalBuffer(float* normalsBuffer, size_t normalsArrSize)
{
	normals.resize(normalsArrSize / sizeof(float));
	memcpy(&normals[0], normalsBuffer, normalsArrSize);

	glGenBuffers(1, (GLuint*)&(normalsId));
	glBindBuffer(GL_ARRAY_BUFFER, normalsId);
	glBufferData(GL_ARRAY_BUFFER, normalsArrSize, normalsBuffer, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void Mesh::InitTexCoordBuffer(float* texBuffer, size_t texArrSize)
{
	texCoords.resize(texArrSize / sizeof(float));
	memcpy(&texCoords[0], texBuffer, texArrSize);

	glGenBuffers(1, (GLuint*)&(texCoordId));
	glBindBuffer(GL_ARRAY_BUFFER, texCoordId);
	glBufferData(GL_ARRAY_BUFFER, texArrSize, texBuffer, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void Mesh::InitIndexBuffer(unsigned int* indexBuffer, size_t indexArrSize)
{
	this->indexArrSize = indexArrSize;

	indices.resize(indexArrSize / sizeof(unsigned int));
	memcpy(&indices[0], indexBuffer, indexArrSize);

	glGenBuffers(1, (GLuint*)&(indexId));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexArrSize, indexBuffer, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


void Mesh::GetAllVectorsSize(unsigned int& vert, unsigned int& norm, unsigned int& ind) const
{
	vert = vertices.size();
	norm = normals.size();
	ind = indices.size();
}


void Mesh::Draw(float* transformMatrix, unsigned int textureId, float* color, bool wireFrameBlack, bool drawVertexNormals, bool drawFaceNormals) const
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


void Mesh::GetAllVertexData(std::vector<float>& vertexArray, std::vector<float>& normalsArray, std::vector<float>& texCoordArray, std::vector<unsigned int>& indicesArray) const
{
	vertexArray = vertices;
	normalsArray = normals;
	texCoordArray = texCoords;
	indicesArray = indices;
}


void Mesh::DrawVertexNormals() const
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


void Mesh::DrawFaceNormals() const
{
	glLineWidth(3.0f);
	glColor3f(0, 1, 0);

	glBegin(GL_LINES);

	for (int i = 0; i < indices.size(); i += 3)
	{
		unsigned int index0 = indices[i];
		unsigned int index1 = indices[i + 1];
		unsigned int index2 = indices[i + 2];

		float3 P0(vertices[index0 * 3], vertices[index0 * 3 + 1], vertices[index0 * 3 + 2]);
		float3 P1(vertices[index1 * 3], vertices[index1 * 3 + 1], vertices[index1 * 3 + 2]);
		float3 P2(vertices[index2 * 3], vertices[index2 * 3 + 1], vertices[index2 * 3 + 2]);

		float3 V0 = P0 - P1;
		float3 V1 = P2 - P1;

		float3 N = V1.Cross(V0);
		N = N.Normalized() * 3;

		float3 center = (P0 + P1 + P2) / 3;

		glVertex3f(center.x, center.y, center.z);
		glVertex3f(center.x + N.x, center.y + N.y, center.z + N.z);
	}

	glEnd();
}