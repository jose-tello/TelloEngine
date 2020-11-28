#include "R_Model.h"
#include "ModelImporter.h"

ModelNode::ModelNode() :
	uid(0),
	parentId(0),
	name(name),
	meshId(0),
	materialId(0)
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

}


void R_Model::UnLoad()
{
	modelNodes.clear();
}


void R_Model::GetModelNodes(std::vector<ModelNode>& nodes)
{
	nodes = modelNodes;
}