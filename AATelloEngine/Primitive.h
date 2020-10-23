#ifndef __PRIMITIVE_H__
#define __PRIMITIVE_H__

#include <vector>

#define PI 3.1415f

namespace Primitive
{
	void CreateCube(std::vector<float>& vertices, std::vector<unsigned int>& indices);
	void CreatePiramid(std::vector<float>& vertices, std::vector<unsigned int>& indices);

	void CreateSphere(float radius, unsigned int rings, unsigned int sectors, std::vector<float>& vertices, std::vector<float>& normals,
		std::vector<float>& texCoords, std::vector<unsigned int>& indices);

	void CreateCilinder(float radius, unsigned int sectorCount, float height, std::vector<float>& vertices, std::vector<float>& normals,
		std::vector<float>& texCoords, std::vector<unsigned int>& indices);
}
#endif // !__PRIMITIVE_H__