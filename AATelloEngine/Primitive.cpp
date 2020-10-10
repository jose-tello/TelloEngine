#include "Primitive.h"
#include "PrimitiveVertex.h"

#include "Glew/include/glew.h"
#pragma comment(lib,"Glew/libx86/glew32.lib")

#include <gl/GL.h>
#include <gl/GLU.h>

Primitive::Primitive() :
	vertexId(0),
	indexId(0),
	vertexArrSize(0),
	indexArrSize(0),

	transform(IdentityMatrix),
	color(1.f, 1.f, 1.f, 1.f)
{
}

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


Primitive::Primitive(vec3& position, float angle, vec3& rotation, float red, float green, float blue, float alpha) :
	vertexId(0),
	indexId(0),
	vertexArrSize(0),
	indexArrSize(0),

	transform(IdentityMatrix),
	color(red, green, blue, alpha)
{
	transform.translate(position.x, position.y, position.z);
	transform.rotate(angle, rotation);
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


void Primitive::SetEscale(vec3& escale)
{
	transform.scale(escale.x, escale.y, escale.z);
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


Sphere::Sphere(float radius, unsigned int rings, unsigned int sectors, vec3& position, float angle, vec3& rotation, float red, float green, float blue, float alpha) :
	Primitive(position, angle, rotation, red, green, blue, alpha)
{

	std::vector<float> vertices;
	std::vector<float> normals;
	std::vector<unsigned int> indices;

	float x, y, z, xy;                              // vertex position
	float nx, ny, nz, lengthInv = 1.0f / radius;    // vertex normal                         

	float sectorStep = 2 * M_PI / sectors;
	float stackStep = M_PI / rings;
	float sectorAngle, stackAngle;


	for (int r = 0; r <= rings; r++)
	{
		float aux = stackStep * r;
		stackAngle = 3.14f / 2 - aux;
		xy = radius * cosf(stackAngle);
		z = radius * sinf(stackAngle);

		for (int s = 0; s <= sectors; s++)
		{
			sectorAngle = s * sectorStep;

			x = xy * cosf(sectorAngle);
			y = xy * sinf(sectorAngle);

			vertices.push_back(x);
			vertices.push_back(y);
			vertices.push_back(z);


			nx = x * lengthInv;
			ny = y * lengthInv;
			nz = z * lengthInv;

			normals.push_back(nx);
			normals.push_back(ny);
			normals.push_back(nz);
		}
	}

	int k1, k2;
	for (int r = 0; r < rings; r++)
	{
		k1 = r * (sectors + 1);
		k2 = k1 + sectors + 1;

		for (int s = 0; s < sectors; ++s, ++k1, ++k2)
		{
			if (r != 0)
			{
				indices.push_back(k1);
				indices.push_back(k2);
				indices.push_back(k1 + 1);
			}

			if (r != (rings - 1))
			{
				indices.push_back(k1 + 1);
				indices.push_back(k2);
				indices.push_back(k2 + 1);
			}
		}
	}

	vertexArrSize = vertices.size();
	indexArrSize = indices.size();
	normalsArrSize = normals.size();

	std::size_t vSize = vertexArrSize * sizeof(float);
	std::size_t nSize = normalsArrSize * sizeof(float);
	std::size_t iSize = indexArrSize * sizeof(unsigned int);

	glGenBuffers(1, (GLuint*)&(vertexId));
	glBindBuffer(GL_ARRAY_BUFFER, vertexId);
	glBufferData(GL_ARRAY_BUFFER, vSize, vertices.data(), GL_STATIC_DRAW);


	glGenBuffers(1, (GLuint*)&(normalsId));
	glBindBuffer(GL_NORMAL_ARRAY, normalsId);
	glBufferData(GL_NORMAL_ARRAY, nSize, normals.data(), GL_STATIC_DRAW);


	glGenBuffers(1, (GLuint*)&(indexId));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, iSize, indices.data(), GL_STATIC_DRAW);

}


void Sphere::Draw() const
{
	glPushMatrix();
	glMultMatrixf(transform.M);
	glColor3f(color.r, color.g, color.b);

	//int idSize = indexArrSize / sizeof(unsigned int);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, vertexId);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glNormalPointer(GL_FLOAT, 0, NULL);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexId);
	glDrawElements(GL_TRIANGLES, indexArrSize, GL_UNSIGNED_INT, NULL);
	glDisableClientState(GL_VERTEX_ARRAY);

	glPopMatrix();
}


Plane::Plane(float x, float y, float z, float d) :
	Primitive(),
	normal(x, y, z),
	constant(d)
{
}


void Plane::Draw()
{
	glLineWidth(1.0f);
	glColor3f(color.r, color.g, color.b);

	glBegin(GL_LINES);

	float d = 200.0f;

	for (float i = -d; i <= d; i += 1.0f)
	{
		glVertex3f(i, 0.0f, -d);
		glVertex3f(i, 0.0f, d);
		glVertex3f(-d, 0.0f, i);
		glVertex3f(d, 0.0f, i);
	}

	glEnd();
}