#pragma once
#include "Module.h"
#include "Globals.h"
#include "Primitive.h"

#include "vector"

#define MAX_SNAKE 2

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	bool Update(float dt);
	bool PostUpdate(float dt);
	bool CleanUp();

	
private:
	std::vector<Primitive*> primitiveVec;
};
