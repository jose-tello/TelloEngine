#ifndef __R_MODEL_H__
#define __R_MODEL_H__

#include "Resource.h"

#include <vector>
#include <string>

struct ModelNode
{
public:
	ModelNode();
	~ModelNode();

public:
	int uid;
	int parentId;
	std::string name;

	float position[3];
	float rotation[4];
	float scale[3];

	
	int meshId;
	int materialId;
};


class R_Model : public Resource 
{
public:
	R_Model(int uid, const char* assetPath, RESOURCE_TYPE type);
	~R_Model() override;

	void Load() override;

	void GetModelNodes(std::vector<ModelNode>& modelNodes) const;
	void SetModelNodes(std::vector<ModelNode>& modelNodes);

	int GetFirstMesh() const;
private:
	std::vector<ModelNode> modelNodes;
};
#endif // !__R_MODEL_H__

