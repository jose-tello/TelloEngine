#ifndef __C_ABERRATION_H__
#define __C_ABERRATION_H__

#include "Component.h"
#include "MathGeoLib/src/MathGeoLib.h"

#include <vector>

class C_Aberration : public Component
{
public:
	C_Aberration();
	~C_Aberration() override;

	void OnUpdateTransform(float4x4& transform) override;

	float GetDeformationX() const;
	float GetDeformationY() const;
	float GetDeformationZ() const;

	void SetDeformationX(float defX);
	void SetDeformationY(float defY);
	void SetDeformationZ(float defZ);

	bool GetDebugDraw() const;
	void SetDebugDraw(bool draw);

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
	int cubeMeshId;		//Cube mesh resource
	
	float deformationX;
	float deformationY;
	float deformationZ;

	bool debugDraw = true;

};


#endif // !__C_ABERRATION_H__

