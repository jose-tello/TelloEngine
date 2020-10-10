#include "Primitive.h"
#include "PrimitiveVertex.h"

#include "Glew/include/glew.h"
#pragma comment(lib,"Glew/libx86/glew32.lib")

#include <gl/GL.h>
#include <gl/GLU.h>


Primitive::Primitive(float* vertexArray, std::size_t vertexSize, unsigned int* indexArray, std::size_t indexSize, vec3& position, float angle, vec3& rotation,
					 float red, float green, float blue, float alpha) :
	vertexId(0),
	indexId(0),
	vertexArrSize(vertexSize),
	indexArrSize(indexSize),

	transform(IdentityMatrix),
	color(red, green, blue, alpha)

{
	transform.translate(position.x, position.y, position.z);
	transform.rotate(angle, rotation);

	glGenBuffers(1, (GLuint*)&(vertexId));
	glBindBuffer(GL_ARRAY_BUFFER, vertexId);
	glBufferData(GL_ARRAY_BUFFER, vertexArrSize, vertexArray, GL_STATIC_DRAW);
	

	glGenBuffers(1, (GLuint*)&(indexId));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexArrSize, indexArray, GL_STATIC_DRAW);
}

Primitive::~Primitive()
{
}


void Primitive::SetPosition(vec3& pos)
{
	transform.translate(pos.x, pos.y, pos.z);
}


void Primitive::SetRotation(float angle, vec3& rotation)
{
	transform.rotate(angle, rotation);
}


void Primitive::Draw() const
{
	glPushMatrix();
	glMultMatrixf(transform.M);
	glColor3f(color.r, color.g, color.b);

	int idSize = indexArrSize / sizeof(unsigned int);

	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, vertexId);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexId);
	glDrawElements(GL_TRIANGLES, idSize, GL_UNSIGNED_INT, NULL);
	glDisableClientState(GL_VERTEX_ARRAY);

	glPopMatrix();
}


Cube::Cube(vec3& position, float angle, vec3& rotation, float red, float green, float blue, float alpha) :
	Primitive(cubeVertexArray, sizeof(cubeVertexArray), cubeIndexArray, sizeof(cubeIndexArray), position, angle, rotation, red, green, blue, alpha)
{
}