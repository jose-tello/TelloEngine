#include "C_PointLight.h"

#include "Application.h"
#include "M_Renderer3D.h"

#include "GameObject.h"
#include "C_Transform.h"
#include "C_Material.h"

#include "UniformHandle.h"

#include "Config.h"

C_PointLight::C_PointLight() : Component(COMPONENT_TYPE::POINT_LIGHT),
	lightIntensity(1.6f),
	specular(30.f)
{
	lightColor[0] = 0.7f;
	lightColor[1] = 0.7f;
	lightColor[2] = 0.7f;

	ambientColor[0] = 0.22f;
	ambientColor[1] = 0.22f;
	ambientColor[2] = 0.22f;

	memset(diffuse, 0, sizeof(diffuse));
}


C_PointLight::~C_PointLight()
{
	App->renderer3D->DeleteLight(this);
}


bool C_PointLight::Update(float dt)
{
	App->renderer3D->PushLight(this);

	return true;
}


void C_PointLight::PushLightUniforms(C_Material* material, int lightNumber)
{
	UniformHandle* uniform = nullptr;
	char buffer[64];

	sprintf(buffer, "lightInfo[%i].light_position", lightNumber);
	uniform = material->GetUniform(buffer);

	if (uniform != nullptr)
	{
		C_Transform* transform = &this->GetOwner()->transform;

		float position[3];
		transform->GetGlobalPos(position[0], position[1], position[2]);
		uniform->SetFloatVec3(position);
	}

	sprintf(buffer, "lightInfo[%i].light_color", lightNumber);
	uniform = material->GetUniform(buffer);

	if (uniform != nullptr)
	{
		uniform->SetFloatVec3(lightColor);
	}

	sprintf(buffer, "lightInfo[%i].ambient_color", lightNumber);
	uniform = material->GetUniform(buffer);
	if (uniform != nullptr)
	{
		uniform->SetFloatVec3(ambientColor);
	}

	sprintf(buffer, "lightInfo[%i].light_intensity", lightNumber);
	uniform = material->GetUniform(buffer);

	if (uniform != nullptr)
	{
		uniform->SetFloat(lightIntensity);
	}

	sprintf(buffer, "lightInfo[%i].specular", lightNumber);
	uniform = material->GetUniform(buffer);

	if (uniform != nullptr)
	{
		uniform->SetFloat(specular);
	}
}


void C_PointLight::Load(Config& node)
{
	ConfigArray lightCol = node.GetArray("light_color");

	for (int i = 0; i < 3; i++)
	{
		Config aux = lightCol.GetNode(i);
		lightColor[i] = aux.GetNum("value");
	}


	ConfigArray ambColor = node.GetArray("ambient_color");

	for (int i = 0; i < 3; i++)
	{
		Config aux = ambColor.GetNode(i);
		ambientColor[i] = aux.GetNum("value");
	}

	ConfigArray diff = node.GetArray("diffuse");

	for (int i = 0; i < 3; i++)
	{
		Config aux = diff.GetNode(i);
		diffuse[i] = aux.GetNum("value");
	}


	specular = node.GetNum("specular");

	lightIntensity = node.GetNum("light_power");
}


void C_PointLight::Save(Config& node) const
{
	node.AppendNum("type", (int)COMPONENT_TYPE::POINT_LIGHT);

	ConfigArray lightCol = node.AppendArray("light_color");

	for (int i = 0; i < 3; i++)
	{
		Config aux = lightCol.AppendNode();
		aux.AppendNum("value", lightColor[i]);
	}

	ConfigArray ambColor = node.AppendArray("ambient_color");

	for (int i = 0; i < 3; i++)
	{
		Config aux = ambColor.AppendNode();
		aux.AppendNum("value", ambientColor[i]);
	}

	ConfigArray diff = node.AppendArray("diffuse");

	for (int i = 0; i < 3; i++)
	{
		Config aux = diff.AppendNode();
		aux.AppendNum("value", diffuse[i]);
	}

	node.AppendNum("specular", specular);

	node.AppendNum("light_power", lightIntensity);
}


void C_PointLight::GetLightColor(float* aux) const
{
	aux[0] = lightColor[0];
	aux[1] = lightColor[1];
	aux[2] = lightColor[2];
}


void C_PointLight::SetLightColor(float* aux)
{
	lightColor[0] = aux[0];
	lightColor[1] = aux[1];
	lightColor[2] = aux[2];
}



void C_PointLight::GetAmbientColor(float* aux) const
{
	aux[0] = ambientColor[0];
	aux[1] = ambientColor[1];
	aux[2] = ambientColor[2];
}


void C_PointLight::SetAmbientColor(float* aux)
{
	ambientColor[0] = aux[0];
	ambientColor[1] = aux[1];
	ambientColor[2] = aux[2];
}


void C_PointLight::GetDiffuse(float* aux) const
{
	aux[0] = diffuse[0];
	aux[1] = diffuse[1];
	aux[2] = diffuse[2];
}


void C_PointLight::SetDiffuse(float* aux)
{
	diffuse[0] = aux[0];
	diffuse[1] = aux[1];
	diffuse[2] = aux[2];
}


float C_PointLight::GetSpecular() const
{
	return specular;
}


void C_PointLight::SetSpecular(float spec)
{
	specular = spec;
}


float C_PointLight::GetLightIntensity() const
{
	return lightIntensity;
}


void C_PointLight::SetLightIntensity(float value)
{
	lightIntensity = value;
}