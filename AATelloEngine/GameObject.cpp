#include "GameObject.h"

#include "Component.h"
#include "C_Mesh.h"
#include "C_Material.h"
#include "C_Camera.h"
#include "C_PointLight.h"

#include "Resource.h"


#include "Config.h"

#include "MathGeoLib/src/Algorithm/Random/LCG.h"

GameObject::GameObject(GameObject* parent, int uuid) :
	parent(parent),
	toDelete(false),
	transform(),
	name(),
	uuid(uuid)
{
	transform.SetOwner(this);
	components.push_back(&transform);

	if (uuid == 0)
	{
		LCG randomNumber;
		this->uuid = randomNumber.IntFast();
	}
}


GameObject::GameObject(std::string& name, GameObject* parent, int uuid) :
	parent(parent),
	toDelete(false),
	transform(),
	name(name),
	uuid(uuid)
{
	transform.SetOwner(this);
	components.push_back(&transform);

	if (uuid == 0)
	{
		LCG randomNumber;
		this->uuid = randomNumber.IntFast();
	}
}


GameObject::~GameObject()
{
	name.clear();
	parent = nullptr;

	int count = components.size();
	for (int i = 1; i < count; i++)
	{
		delete components[i];
		components[i] = nullptr;
	}
	components.clear();

	count = childs.size();
	for (int i = 0; i < count; i++)
	{
		delete childs[i];
		childs[i] = nullptr;
	}
	childs.clear();

	name.clear();

	uuid = 0;
}


Component* GameObject::GetComponent(COMPONENT_TYPE componentType) const
{
	int componentsCount = components.size();
	for (int i = 0; i < componentsCount; i++)
	{
		if (components[i]->GetType() == componentType)
			return components[i];
	}

	return nullptr;
}


void GameObject::GetAllComponents(std::vector<Component*>& vec) const
{
	vec = components;
}


bool GameObject::AddComponent(Component* component)
{
	if (CheckNotRepeated(component->GetType()) == false && component->SetOwner(this) == true)
		return false;
	
	components.push_back(component);
	component->SetOwner(this);
	
	return true;
}


bool GameObject::DeleteComponent(COMPONENT_TYPE componentType)
{
	int componentsCount = components.size();
	for (int i = 0; i < componentsCount; i++)
	{
		if (components[i]->GetType() == componentType)
		{
			delete components[i];
			components.erase(components.begin() + i);

			return true;
		}
	}

	return false;
}


void GameObject::AddResource(int resourceUid, int resourceType)
{
	switch ((RESOURCE_TYPE)resourceType)
	{
	case RESOURCE_TYPE::MESH:
	{
		C_Mesh* mesh;
		Component* meshComp = GetComponent(COMPONENT_TYPE::MESH);
		if (meshComp == nullptr)
		{
			mesh = new C_Mesh();
			AddComponent(mesh);
		}
		else
			mesh = (C_Mesh*)meshComp;

		mesh->SetMesh(resourceUid);
	}
		break;

	case RESOURCE_TYPE::MATERIAL:
	{
		C_Material* material;
		Component* mat = GetComponent(COMPONENT_TYPE::MATERIAL);
		if (mat == nullptr)
		{
			material = new C_Material();
			AddComponent(material);
		}
		else
			material = (C_Material*)mat;

		material->SetMaterial(resourceUid);
	}

		break;

	case RESOURCE_TYPE::TEXTURE:
	{
		C_Material* material;
		Component* mat = GetComponent(COMPONENT_TYPE::MATERIAL);
		if (mat == nullptr)
		{
			material = new C_Material();
			AddComponent(material);
		}
		else
			material = (C_Material*)mat;

		material->SetTexture(resourceUid);
	}
		break;

	case RESOURCE_TYPE::SHADER:
	{
		C_Material* material;
		Component* mat = GetComponent(COMPONENT_TYPE::MATERIAL);
		if (mat == nullptr)
		{
			material = new C_Material();
			AddComponent(material);
		}
		else
			material = (C_Material*)mat;

		material->SetShader(resourceUid);
	}
	break;

	default:
		assert("passed an incorrect type of resource");
		break;
	}
}


void GameObject::Update(float dt)
{
	int componentsCount = components.size();
	for (int i = 0; i < componentsCount; i++)
	{
		components[i]->Update(dt);
	}
}


//TODO: this should be called by the event system
void GameObject::OnUpdateTransform(float4x4& globalTransform)
{
	int componentsCount = components.size();
	for (int i = 0; i < componentsCount; i++)
	{
		components[i]->OnUpdateTransform(globalTransform);
	}
}


const char* GameObject::GetName() const
{
	return name.c_str();
}


void GameObject::SetName(const char* n)
{
	name = n;
}


int GameObject::GetUuid() const
{
	return uuid;
}


//Does not delete the child
void GameObject::RemoveChild(int uid)
{
	for (int i = 0; i < childs.size(); i++)
	{
		if (childs[i]->GetUuid() == uid)
		{
			childs.erase(childs.begin() + i);
			return;
		}
	}
}


//TODO: news should be handled from outside
void GameObject::Load(Config& node)
{
	ConfigArray componentsArray = node.GetArray("components");
	int componentsCount = componentsArray.GetSize();

	for (int i = 0; i < componentsCount; i++)
	{
		Config componentNode = componentsArray.GetNode(i);
		int type = componentNode.GetNum("type");

		switch (COMPONENT_TYPE(type))
		{
		case COMPONENT_TYPE::TRANSFORM:
			transform.Load(componentNode);
			break;

		case COMPONENT_TYPE::MESH:
		{
			C_Mesh* mesh = new C_Mesh();
			mesh->Load(componentNode);
			AddComponent(mesh);
		}
			break;

		case COMPONENT_TYPE::MATERIAL:
		{
			C_Material* material = new C_Material();
			material->Load(componentNode);
			AddComponent(material);
		}
			break;

		case COMPONENT_TYPE::CAMERA:
		{
			C_Camera* camera = new C_Camera();
			camera->Load(componentNode);
			AddComponent(camera);
		}
			break;

		case COMPONENT_TYPE::POINT_LIGHT:
		{
			C_PointLight* lightSource = new C_PointLight();
			lightSource->Load(componentNode);
			AddComponent(lightSource);
		}
			break;

		default:
			assert("Invalid component type");
			break;
		}
	}
}


void GameObject::Save(Config& node) const
{
	node.AppendNum("uuid", uuid);

	if (parent != nullptr)
		node.AppendNum("parent", parent->GetUuid());

	else
		node.AppendNum("parent", 0);

	node.AppendString("name", name.c_str());

	ConfigArray componentsArray = node.AppendArray("components");

	int componentsCount = components.size();
	for (int i = 0; i < componentsCount; i++)
	{
		Config componentNode = componentsArray.AppendNode();
		components[i]->Save(componentNode);
	}
}


bool GameObject::CheckNotRepeated(COMPONENT_TYPE componentType)
{
	int componentsCount = components.size();
	for (int i = 0; i < componentsCount; i++)
	{
		if (components[i]->GetType() == componentType)
			return false;
	}

	return true;
}