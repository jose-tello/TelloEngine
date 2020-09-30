#include "Globals.h"
#include "glmath.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{


	return true;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	int vecSize = primitiveVec.size();
	for (uint n = 0; n < vecSize; n++)
	{
		delete primitiveVec[n];
		primitiveVec[n] = nullptr;
	}

	return true;
}

// Update
bool ModuleSceneIntro::Update(float dt)
{

	Plane p(0, 1, 0, 0);
	float scale = 3; //change this to scale ground plane
	p.Scale(scale, 0, scale);
	p.axis = true;
	p.Render();


	int vecSize = primitiveVec.size();
	for (uint n = 0; n < vecSize; n++)
	{
		primitiveVec[n]->Update();
	}

	return true;
}


bool ModuleSceneIntro::PostUpdate(float dt)
{
	int vecSize = primitiveVec.size();
	for (uint n = 0; n < vecSize; n++)
	{
		primitiveVec[n]->Render();
	}

	return true;
}