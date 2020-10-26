#ifndef __C_MESH_H__
#define __C_MESH_H__

#include "Component.h"
#include <vector>

class C_Mesh : public Component
{
public:
	C_Mesh(GameObject* owner);
	~C_Mesh() override;

	//Primitive things
	void InitAsCube();

	void InitVertexBuffer(float*, size_t);
	void InitNormalBuffer(float*, size_t);
	void InitTexCoordBuffer(float*, size_t);
	void InitIndexBuffer(unsigned int*, size_t);

	void Draw(float* transformMatrix, unsigned int textureId, float* col) const;
	void DrawVertexNormals() const;
	void DrawFaceNormals() const;


private:
	unsigned int vertexId;
	unsigned int normalsId;
	unsigned int texCoordId;
	unsigned int indexId;

	//Used to draw vertex and face normals, mayby i should get rid of them
	std::vector<float> vertices;
	std::vector<float> normals;
	std::vector<unsigned int> indices;

	int indexArrSize;
	
};


#endif // !__C_MESH_H__

