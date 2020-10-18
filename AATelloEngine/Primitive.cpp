#include "Primitive.h"
#include "PrimitiveVertex.h"

#include "Mesh.h"

#include "Glew/include/glew.h"
#pragma comment(lib,"Glew/libx86/glew32.lib")

#include <gl/GL.h>
#include <gl/GLU.h>

Primitive::Primitive() :
	mesh(nullptr)
{
}


Primitive::Primitive(float* vertexArray, std::size_t vertexSize, unsigned int* indexArray, std::size_t indexSize, vec3& position, float angle, vec3& rotation,
					 float red, float green, float blue, float alpha) :
	mesh(nullptr)
{
	mesh = new Mesh(vertexArray, vertexSize, indexArray, indexSize, position, angle, rotation, red, green, blue, alpha);
}


Primitive::~Primitive()
{
	mesh = nullptr;
}


void Primitive::SetPosition(vec3& pos)
{
	mesh->SetPosition(pos);
}


void Primitive::SetRotation(float angle, vec3& rotation)
{
	mesh->SetRotation(angle, rotation);
}


void Primitive::SetEscale(vec3& escale)
{
	mesh->SetEscale(escale);
}


Mesh* Primitive::GetMesh() const
{
	return mesh;
}


Cube::Cube(vec3& position, float angle, vec3& rotation, float red, float green, float blue, float alpha) :
	Primitive(cubeVertexArray, sizeof(cubeVertexArray), cubeIndexArray, sizeof(cubeIndexArray), position, angle, rotation, red, green, blue, alpha)
{
}


Cube::~Cube()
{
	mesh = nullptr;
}


Piramid::Piramid(vec3& position, float angle, vec3& rotation, float red, float green, float blue, float alpha) :
	Primitive(piramidVertex, sizeof(piramidVertex), piramidIndex, sizeof(piramidIndex), position, angle, rotation, red, green, blue, alpha)
{
}


Piramid::~Piramid()
{
	mesh = nullptr;
}


Sphere::Sphere(float radius, unsigned int rings, unsigned int sectors, vec3& position, float angle, vec3& rotation, float red, float green, float blue, float alpha) :
	Primitive()
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
	std::size_t indexArrSize = indices.size() * sizeof(unsigned int);

	mesh = new Mesh(&vertices[0], vSize, &indices[0], indexArrSize, position, angle, rotation, red, green, blue, alpha);
}


Sphere::~Sphere()
{
	mesh = nullptr;
}


Cilinder::Cilinder(unsigned int sectorCount, float height, float radius, vec3& position, float angle, vec3& rotation,
				   float red, float green, float blue, float alpha) :
	Primitive()
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
	std::size_t indexArrSize = indices.size() * sizeof(unsigned int);

	mesh = new Mesh(&vertices[0], vSize, &indices[0], indexArrSize, position, angle, rotation, red, green, blue, alpha);
}


Cilinder::~Cilinder()
{
	mesh = nullptr;
}


Plane::Plane(float x, float y, float z, float d) :
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
	glColor3f(1, 1, 1);

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