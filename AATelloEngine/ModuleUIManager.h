#ifndef __ModuleUIManager_H__
#define __ModuleUIManager_H__

#include "Module.h"

#include <string>
#include <vector>


class ModuleUIManager : public Module
{
public:

	ModuleUIManager(Application* app, bool start_enabled = true);
	~ModuleUIManager();

	bool Init();
	bool Start();

	bool PreUpdate(float dt);
	bool PostUpdate(float dt);

	bool CleanUp();

};

#endif // __ModuleAudio_H__
