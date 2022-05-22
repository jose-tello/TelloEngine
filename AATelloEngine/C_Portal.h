#ifndef __C_PORTAL_H__
#define __C_PORTAL_H__

#include "Component.h"
#include "MathGeoLib/src/MathGeoLib.h"

#include <vector>

class C_Portal : public Component
{
public:
	C_Portal();
	~C_Portal() override;

	bool PreUpdate(float dt) override;

	void OnUpdateTransform(float4x4& transform) override;

	bool Connect(int uid);
	void Disconnect();
	int GetConnection() const;

	bool GetDebugDraw() const;
	void SetDebugDraw(bool draw);

	float* GetDebugDrawColor();

	unsigned int GetVAO() const;
	void GetVertexArray(std::vector<float>& vertexArray) const;
	void GetAllVertexData(std::vector<float>& vertexArray, std::vector<float>& normalsArray, std::vector<float>& texCoordArray, std::vector<unsigned int>& indicesArray) const;
	void GetAllVectorsSize(unsigned int& vertexCount, unsigned int& normalsCount, unsigned int& indexCount) const;

	unsigned int GetIndicesSize() const;
	unsigned int GetIndexOffset() const;
	unsigned int GetVertexOffset() const;

	const float* GetAABBMinPoint() const;
	const float* GetAABBMaxPoint() const;

	void Load(Config& node) override;
	void Save(Config& node) const override;

private:
	AABB aabb;
	int planeMeshId;		//Cube mesh resource

	int connectedGO;

	bool debugDraw = true;
	float3 debugDrawColor;

};


#endif // !__C_PORTAL_H__