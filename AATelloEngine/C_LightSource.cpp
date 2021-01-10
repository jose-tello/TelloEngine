#include "C_LightSource.h"

#include "Application.h"
#include "M_Renderer3D.h"

#include "Config.h"

C_LightSource::C_LightSource() : Component(COMPONENT_TYPE::LIGHT_SOURCE),
	lightPower(0.f)
{
	memset(lightColor, 0, sizeof(lightColor));
	memset(ambientColor, 0, sizeof(ambientColor));
	memset(diffuse, 0, sizeof(diffuse));
	memset(specular, 0, sizeof(specular));
}


C_LightSource::~C_LightSource()
{
	App->renderer3D->DeleteLight(this);
}


bool C_LightSource::Update(float dt)
{
	App->renderer3D->PushLight(this);

	return true;
}


void C_LightSource::Load(Config& node)
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


	ConfigArray spec = node.GetArray("specular");

	for (int i = 0; i < 3; i++)
	{
		Config aux = diff.GetNode(i);
		specular[i] = aux.GetNum("value");
	}

	lightPower = node.GetNum("light_power");
}


void C_LightSource::Save(Config& node) const
{
	node.AppendNum("type", (int)COMPONENT_TYPE::LIGHT_SOURCE);

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


	ConfigArray spec = node.AppendArray("specular");

	for (int i = 0; i < 3; i++)
	{
		Config aux = spec.AppendNode();
		aux.AppendNum("value", specular[i]);
	}

	node.AppendNum("light_power", lightPower);
}


void C_LightSource::GetLightColor(float* aux) const
{
	aux[0] = lightColor[0];
	aux[1] = lightColor[1];
	aux[2] = lightColor[2];
}


void C_LightSource::SetLightColor(float* aux)
{
	lightColor[0] = aux[0];
	lightColor[1] = aux[1];
	lightColor[2] = aux[2];
}



void C_LightSource::GetAmbientColor(float* aux) const
{
	aux[0] = ambientColor[0];
	aux[1] = ambientColor[1];
	aux[2] = ambientColor[2];
}


void C_LightSource::SetAmbientColor(float* aux)
{
	ambientColor[0] = aux[0];
	ambientColor[1] = aux[1];
	ambientColor[2] = aux[2];
}


void C_LightSource::GetDiffuse(float* aux) const
{
	aux[0] = diffuse[0];
	aux[1] = diffuse[1];
	aux[2] = diffuse[2];
}


void C_LightSource::SetDiffuse(float* aux)
{
	diffuse[0] = aux[0];
	diffuse[1] = aux[1];
	diffuse[2] = aux[2];
}


void C_LightSource::GetSpecular(float* aux) const
{
	aux[0] = specular[0];
	aux[1] = specular[1];
	aux[2] = specular[2];
}


void C_LightSource::SetSpecular(float* aux)
{
	specular[0] = aux[0];
	specular[1] = aux[1];
	specular[2] = aux[2];
}


float C_LightSource::GetLightPower() const
{
	return lightPower;
}


void C_LightSource::SetLightPower(float value)
{
	lightPower = value;
}