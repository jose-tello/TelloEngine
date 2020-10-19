#ifndef __PRIMITIVE_H__
#define __PRIMITIVE_H__

#include "PrimitiveVertex.h"

#include <vector>

namespace Primitive
{
	void CreateCube(std::vector<float>& vertices, std::vector<unsigned int>& indices)
	{
		vertices.resize(sizeof(cubeVertexArray) / sizeof(float));
		memcpy(&vertices[0], cubeVertexArray, sizeof(cubeVertexArray));

		indices.resize(sizeof(cubeIndexArray) / sizeof(unsigned int));
		memcpy(&indices[0], cubeIndexArray, sizeof(cubeIndexArray));
	}

	void CreatePiramid(std::vector<float>& vertices, std::vector<unsigned int>& indices)
	{
		vertices.resize(sizeof(piramidVertexArray) / sizeof(float));
		memcpy(&vertices[0], piramidVertexArray, sizeof(piramidVertexArray));

		indices.resize(sizeof(piramidIndexArray) / sizeof(unsigned int));
		memcpy(&indices[0], piramidIndexArray, sizeof(piramidIndexArray));
	}

	void CreateSphere(float radius, unsigned int rings, unsigned int sectors, std::vector<float>& vertices, std::vector<float>& normals, 
					  std::vector<float>& texCoords, std::vector<unsigned int>& indices)
	{
		float x, y, z, xy;                              // vertex position
		float nx, ny, nz, lengthInv = 1.0f / radius;    // vertex normal                         
		float st, tt;

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

				//vertices
				x = xy * cosf(sectorAngle);
				y = xy * sinf(sectorAngle);

				vertices.push_back(x);
				vertices.push_back(y);
				vertices.push_back(z);

				//normals
				nx = x * lengthInv;
				ny = y * lengthInv;
				nz = z * lengthInv;

				normals.push_back(nx);
				normals.push_back(ny);
				normals.push_back(nz);

				//texture coords
				st = (float)s / sectors;
				tt = (float)r / rings;

				texCoords.push_back(st);
				texCoords.push_back(tt);
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
	}

	void CreateCilinder(float radius, unsigned int sectorCount, float height, std::vector<float>& vertices, std::vector<float>& normals,
					  std::vector<float>& texCoords, std::vector<unsigned int>& indices)
	{
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
				//vertices
				float ux = unitVertices[k];
				float uy = unitVertices[k + 1];
				float uz = unitVertices[k + 2];

				vertices.push_back(ux * radius);
				vertices.push_back(uy * radius);
				vertices.push_back(h);

				//normals
				normals.push_back(ux);
				normals.push_back(uy);
				normals.push_back(uz);

				//tex coords
				texCoords.push_back((float)j / sectorCount);
				texCoords.push_back(t);
			}
		}

		int baseCenterIndex = vertices.size() / 3;
		int topCenterIndex = baseCenterIndex + sectorCount + 1;

		for (int i = 0; i < 2; i++)
		{
			float h = -height * .5f + i * height;
			float nz = -1 + i * 2;

			//Center point
			vertices.push_back(0);	vertices.push_back(0);	vertices.push_back(h);
			normals.push_back(0);	normals.push_back(0);	normals.push_back(nz);
			texCoords.push_back(0.5f);	texCoords.push_back(0.5f);

			for (int j = 0, k = 0; j < sectorCount; j++, k += 3)
			{
				float ux = unitVertices[k];
				float uy = unitVertices[k + 1];

				vertices.push_back(ux * radius);
				vertices.push_back(uy * radius);
				vertices.push_back(h);

				normals.push_back(0);
				normals.push_back(0);
				normals.push_back(nz);

				texCoords.push_back(-ux * 0.5f + 0.5f);
				texCoords.push_back(-uy * 0.5f + 0.5f);
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
	}
}
#endif // !__PRIMITIVE_H__