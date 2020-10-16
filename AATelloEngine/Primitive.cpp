#include "Primitive.h"
#include "PrimitiveVertex.h"

#include "Glew/include/glew.h"
#pragma comment(lib,"Glew/libx86/glew32.lib")

#include <gl/GL.h>
#include <gl/GLU.h>

Primitive::Primitive() :
	vertexId(0),
	indexId(0),
	indexArrSize(0),

	transform(IdentityMatrix),
	color(1.f, 1.f, 1.f, 1.f)
{
}

Primitive::Primitive(float* vertexArray, std::size_t vertexSize, unsigned int* indexArray, std::size_t indexSize, vec3& position, float angle, vec3& rotation,
					 float red, float green, float blue, float alpha) :
	vertexId(0),
	indexId(0),
	indexArrSize(indexSize),

	transform(IdentityMatrix),
	color(red, green, blue, alpha)

{
	transform.translate(position.x, position.y, position.z);
	transform.rotate(angle, rotation);

	GenVertexBuffer(vertexArray, vertexSize);
	GenIndexBuffer(indexArray);
}


Primitive::Primitive(vec3& position, float angle, vec3& rotation, float red, float green, float blue, float alpha) :
	vertexId(0),
	indexId(0),
	indexArrSize(0),

	transform(IdentityMatrix),
	color(red, green, blue, alpha)
{
	transform.translate(position.x, position.y, position.z);
	transform.rotate(angle, rotation);
}


Primitive::~Primitive()
{
	glDeleteBuffers(1, &vertexId);
	glDeleteBuffers(1, &indexId);

	vertexId = 0;
	indexId = 0;
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


void Primitive::GenVertexBuffer(float* vertexArray, std::size_t vertexArrSize)
{
	glGenBuffers(1, (GLuint*)&(vertexId));
	glBindBuffer(GL_ARRAY_BUFFER, vertexId);
	glBufferData(GL_ARRAY_BUFFER, vertexArrSize, vertexArray, GL_STATIC_DRAW);
}


void Primitive::GenIndexBuffer(unsigned int* indexArray)
{
	glGenBuffers(1, (GLuint*)&(indexId));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexArrSize, indexArray, GL_STATIC_DRAW);
}


Cube::Cube(vec3& position, float angle, vec3& rotation, float red, float green, float blue, float alpha) :
	Primitive(cubeVertexArray, sizeof(cubeVertexArray), cubeIndexArray, sizeof(cubeIndexArray), position, angle, rotation, red, green, blue, alpha)
{
}


Piramid::Piramid(vec3& position, float angle, vec3& rotation, float red, float green, float blue, float alpha) :
	Primitive(piramidVertex, sizeof(piramidVertex), piramidIndex, sizeof(piramidIndex), position, angle, rotation, red, green, blue, alpha)
{
}


Sphere::Sphere(float radius, unsigned int rings, unsigned int sectors, vec3& position, float angle, vec3& rotation, float red, float green, float blue, float alpha) :
	Primitive(position, angle, rotation, red, green, blue, alpha)
{

	std::vector<float> vertices;
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

	 
	std::size_t vSize = vertices.size() * sizeof(float);
	indexArrSize = indices.size() * sizeof(unsigned int);

	GenVertexBuffer(&vertices[0], vSize);
	
	GenIndexBuffer(&indices[0]);
}


Sphere::~Sphere()
{
}


Cilinder::Cilinder(unsigned int sectorCount, float height, float radius, vec3& position, float angle, vec3& rotation,
				   float red, float green, float blue, float alpha) :
	Primitive(position, angle, rotation, red, green, blue, alpha)
{
	std::vector<float> vertices;
	std::vector<float> normals;
	std::vector<unsigned int> indices;

	std::vector<float> unitVertices;

	float sectorStep = 2 * M_PI / sectorCount;
	float sectorAngle;

	for (int i = 0; i <= sectorCount; i++)
	{
		sectorAngle = i * sectorStep;
		unitVertices.push_back(cos(sectorAngle));
		unitVertices.push_back(sin(sectorAngle));
		unitVertices.push_back(0);
	}

	for (int i = 0; i < 2; i++)
	{
		float h = -height * .5f + i * height;
		float t = 1 - i;

		for (int j = 0, k = 0; j <= sectorCount; j++, k += 3)
		{
			float ux = unitVertices[k];
			float uy = unitVertices[k + 1];
			float uz = unitVertices[k + 2];

			vertices.push_back(ux * radius);
			vertices.push_back(uy * radius);
			vertices.push_back(h);
		}
	}

	int baseCenterIndex = vertices.size() / 3;
	int topCenterIndex = baseCenterIndex + sectorCount + 1;

	for (int i = 0; i < 2; i++)
	{
		float h = -height * .5f + i * height;
		float nz = -1 + i * 2;

		//Center point
		vertices.push_back(0); 
		vertices.push_back(0); 
		vertices.push_back(h);

		for (int j = 0, k = 0; j < sectorCount; j++, k += 3)
		{
			float ux = unitVertices[k];
			float uy = unitVertices[k + 1];

			vertices.push_back(ux * radius);
			vertices.push_back(uy * radius);
			vertices.push_back(h);
		}
	}

	int k1 = 0;
	int k2 = sectorCount + 1;

	for (int i = 0; i < sectorCount; ++i, ++k1, ++k2)
	{
		// 2 triangles per sector
		// k1 => k1+1 => k2
		indices.push_back(k1);
		indices.push_back(k1 + 1);
		indices.push_back(k2);

		// k2 => k1+1 => k2+1
		indices.push_back(k2);
		indices.push_back(k1 + 1);
		indices.push_back(k2 + 1);
	}

	// indices for the base surface
	//NOTE: baseCenterIndex and topCenterIndices are pre-computed during vertex generation
	//      please see the previous code snippet
	for (int i = 0, k = baseCenterIndex + 1; i < sectorCount; ++i, ++k)
	{
		if (i < sectorCount - 1)
		{
			indices.push_back(baseCenterIndex);
			indices.push_back(k + 1);
			indices.push_back(k);
		}
		else // last triangle
		{
			indices.push_back(baseCenterIndex);
			indices.push_back(baseCenterIndex + 1);
			indices.push_back(k);
		}
	}

	// indices for the top surface
	for (int i = 0, k = topCenterIndex + 1; i < sectorCount; ++i, ++k)
	{
		if (i < sectorCount - 1)
		{
			indices.push_back(topCenterIndex);
			indices.push_back(k);
			indices.push_back(k + 1);
		}
		else // last triangle
		{
			indices.push_back(topCenterIndex);
			indices.push_back(k);
			indices.push_back(topCenterIndex + 1);
		}
	}

	std::size_t vSize = vertices.size() * sizeof(float);
	indexArrSize = indices.size() * sizeof(unsigned int);

	GenVertexBuffer(&vertices[0], vSize);

	GenIndexBuffer(&indices[0]);

}


Cilinder::~Cilinder()
{
}


Plane::Plane(float x, float y, float z, float d) :
	Primitive(),
	normal(x, y, z),
	constant(d)
{
}


Plane::~Plane()
{
}


void Plane::Draw() const
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