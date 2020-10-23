#include "Mesh.h"
#include "Primitive.h"

#include "Application.h"
#include "M_Editor.h"

#include "Glew/include/glew.h"
#pragma comment(lib,"Glew/libx86/glew32.lib")

#include <gl/GL.h>
#include <gl/GLU.h>

MeshEntry::MeshEntry() :
	vertexId(0),
	normalsId(0),
	indexId(0),
	texCoordId(0),
	indexArrSize(0)
	
{
}


MeshEntry::MeshEntry(const MeshEntry& copy) :
	vertexId(0),
	normalsId(0),
	indexId(0),
	texCoordId(0),

	vertices(copy.vertices),
	normals(copy.normals),
	texCoords(copy.texCoords),
	indices(copy.indices),
	indexArrSize(0)
{
	if (vertices.size() != 0)
		InitVertexBuffer(&vertices[0], vertices.size() * sizeof(float));
	
	if (normals.size() != 0)
		InitNormalBuffer(&normals[0], normals.size() * sizeof(float));
	
	if (texCoords.size() != 0)
		InitTexCoordBuffer(&texCoords[0], texCoords.size() * sizeof(float));
	
	if (indices.size() != 0)
		InitIndexBuffer(&indices[0], indices.size() * sizeof(unsigned int));
}


MeshEntry::~MeshEntry()
{
	glDeleteBuffers(1, &vertexId);
	glDeleteBuffers(1, &normalsId);	
	glDeleteBuffers(1, &indexId);
	glDeleteBuffers(1, &texCoordId);
	
	vertexId = 0;
	normalsId = 0;
	indexId = 0;
	texCoordId = 0;
}


void MeshEntry::InitAsCube()
{
	Primitive::CreateCube(vertices, indices);

	InitVertexBuffer(&vertices[0], vertices.size() * sizeof(float));
	InitIndexBuffer(&indices[0], indices.size() * sizeof(unsigned int));
}


void MeshEntry::InitAsPiramid()
{
	Primitive::CreatePiramid(vertices, indices);

	InitVertexBuffer(&vertices[0], vertices.size() * sizeof(float));
	InitIndexBuffer(&indices[0], indices.size() * sizeof(unsigned int));
}


void MeshEntry::InitAsSphere(float radius, unsigned int rings, unsigned int sectors)
{
	Primitive::CreateSphere(radius, rings, sectors, vertices, normals, texCoords, indices);

	InitVertexBuffer(&vertices[0], vertices.size() * sizeof(float));
	InitNormalBuffer(&normals[0], normals.size() * sizeof(float));
	InitTexCoordBuffer(&texCoords[0], texCoords.size() * sizeof(float));
	InitIndexBuffer(&indices[0], indices.size() * sizeof(unsigned int));
}


void MeshEntry::InitAsCilinder(float radius, unsigned int sectors, float height)
{
	Primitive::CreateCilinder(radius, sectors, height,vertices, normals, texCoords, indices);

	InitVertexBuffer(&vertices[0], vertices.size() * sizeof(float));
	InitNormalBuffer(&normals[0], normals.size() * sizeof(float));
	InitTexCoordBuffer(&texCoords[0], texCoords.size() * sizeof(float));
	InitIndexBuffer(&indices[0], indices.size() * sizeof(unsigned int));
}


void MeshEntry::InitVertexBuffer(float* vertexBuffer, std::size_t vertexArrSize)
{
	vertices.resize(vertexArrSize / sizeof(float));
	memcpy(&vertices[0], vertexBuffer, vertexArrSize);

	glGenBuffers(1, (GLuint*)&(vertexId));
	glBindBuffer(GL_ARRAY_BUFFER, vertexId);
	glBufferData(GL_ARRAY_BUFFER, vertexArrSize, vertexBuffer, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void MeshEntry::InitNormalBuffer(float* normalsBuffer, std::size_t normalsArrSize)
{
	normals.resize(normalsArrSize / sizeof(float));
	memcpy(&normals[0], normalsBuffer, normalsArrSize);

	glGenBuffers(1, (GLuint*)&(normalsId));
	glBindBuffer(GL_ARRAY_BUFFER, normalsId);
	glBufferData(GL_ARRAY_BUFFER, normalsArrSize, normalsBuffer, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void MeshEntry::InitTexCoordBuffer(float* texBuffer, std::size_t texArrSize)
{
	texCoords.resize(texArrSize / sizeof(float));
	memcpy(&texCoords[0], texBuffer, texArrSize);

	glGenBuffers(1, (GLuint*)&(texCoordId));
	glBindBuffer(GL_ARRAY_BUFFER, texCoordId);
	glBufferData(GL_ARRAY_BUFFER, texArrSize, texBuffer, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void MeshEntry::InitIndexBuffer(unsigned int* indexBuffer, std::size_t indexArrSize)
{
	this->indexArrSize = indexArrSize;

	indices.resize(indexArrSize / sizeof(unsigned int));
	memcpy(&indices[0], indexBuffer, indexArrSize);

	glGenBuffers(1, (GLuint*)&(indexId));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexArrSize, indexBuffer, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


void MeshEntry::Draw(unsigned int textureId) const
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

}


void MeshEntry::DrawVertexNormals() const
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


void MeshEntry::DrawFaceNormals() const
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


Mesh::Mesh(std::vector<MeshEntry>& vec) :
	meshEntryVector(vec),
	transform(),
	color(Red),
	textureId(0)
{
}


//copy constructor
Mesh::Mesh(const Mesh& meshCopy) :
	meshEntryVector(meshCopy.meshEntryVector),
	color(meshCopy.color),
	transform(meshCopy.transform),
	textureId(0)
{
}


//Constructor for primitives
Mesh::Mesh(PRIMITIVE_TYPE type, Color color, float radius, unsigned int rings, unsigned int sectors, float height) :
	color(color),
	transform()
{
	meshEntryVector.push_back(MeshEntry());
	
	switch (type)
	{
	case PRIMITIVE_TYPE::NONE:
		App->editor->AddLog("ERROR: No primitive tipe detected");
		break;

	case PRIMITIVE_TYPE::CUBE:
		meshEntryVector[0].InitAsCube();
		if (radius != 0 || rings != 0 || sectors != 0 || height != 0)
			App->editor->AddLog("WARNING: Using wrong variables");
		break;

	case PRIMITIVE_TYPE::PIRAMID:
		meshEntryVector[0].InitAsPiramid();
		if (radius != 0 || rings != 0 || sectors != 0 || height != 0)
			App->editor->AddLog("WARNING: Using wrong variables");
		break;

	case PRIMITIVE_TYPE::SPHERE:
		if (radius == 0 || rings == 0 || sectors == 0)
		{
			App->editor->AddLog("ERROR: Can not create primitive, using wrong variables");
			return;
		}

		meshEntryVector[0].InitAsSphere(radius, rings, sectors);;
		
		if (height != 0)
			App->editor->AddLog("WARNING: Using wrong variables");
		break;

	case PRIMITIVE_TYPE::CILINDER:
		if (radius == 0 || sectors == 0 || height == 0)
		{
			App->editor->AddLog("ERROR: Can not create primitive, using wrong variables");
			return;
		}

		meshEntryVector[0].InitAsCilinder(radius, sectors, height);
		
		if (rings != 0)
			App->editor->AddLog("WARNING: Using wrong variables");
		break;
	}
}



Mesh::~Mesh()
{
	glDeleteTextures(1, &textureId);
	meshEntryVector.clear();
}


void Mesh::InitTexture(unsigned int texId)
{
	textureId = texId;
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


void Mesh::Draw(bool drawVertexNormals, bool drawFaceNormals, bool black) const
{
	glPushMatrix();
	glMultMatrixf(transform.M);

	int meshEntryCount = meshEntryVector.size();

	for (int i = 0; i < meshEntryCount; i++)
	{
		if (black == true)
			glColor3f(0, 0, 0);

		else
			glColor3f(color.r, color.g, color.b);

		meshEntryVector[i].Draw(textureId);

		if (drawVertexNormals)
			meshEntryVector[i].DrawVertexNormals();
		
		if (drawFaceNormals)
			meshEntryVector[i].DrawFaceNormals();
	}

	glPopMatrix();
}