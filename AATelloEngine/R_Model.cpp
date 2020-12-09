#include "R_Model.h"
#include "ModelImporter.h"

ModelNode::ModelNode() :
	uid(0),
	parentId(0),
	name(name),
	meshId(-1),
	materialId(-1)
{}


ModelNode::~ModelNode()
{
}


R_Model::R_Model(int uid, const char* assetPath, RESOURCE_TYPE type) : Resource(uid, assetPath, type)
{
}


R_Model::~R_Model()
{
	modelNodes.clear();
}


void R_Model::Load()
{
	ModelImporter::Load(this);
	
	AddReference();

	isLoaded = true;
}


void R_Model::GetModelNodes(std::vector<ModelNode>& modelNodes) const
{
	modelNodes = this->modelNodes;
}


void R_Model::SetModelNodes(std::vector<ModelNode>& modelNodes)
{
	this->modelNodes = modelNodes;
}