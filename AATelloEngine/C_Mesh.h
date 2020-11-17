#ifndef __C_MESH_H__
#define __C_MESH_H__

#include "Component.h"
#include <vector>

struct Mesh;

class C_Mesh : public Component
{
public:
	C_Mesh();
	C_Mesh(std::vector<float>& vertexBuff, std::vector<float>& normals, std::vector<float> texCoords, std::vector<unsigned int> indices);
	~C_Mesh() override;

	void Draw(float* transformMatrix, unsigned int textureId, float* col, bool wireframeMode) const;

	void SetMesh(Mesh*);
	Mesh* GetMesh() const;
	void GetAllVectorsSize(unsigned int&, unsigned int&, unsigned int&) const;

	void Load(Config&) override;
	void Save(Config&) const override;

public:
	bool drawVertexNormals;
	bool drawFaceNormals;

private:
	Mesh* mesh = nullptr;
	
};


#endif // !__C_MESH_H__

