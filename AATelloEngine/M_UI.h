#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"

#include <vector>

#define MAX_LOG_SIZE 80

class M_UI : public Module
{
public:
	M_UI(bool start_enabled = true);
	~M_UI();

	bool Init();
	UPDATE_STATUS PreUpdate(float dt);
	UPDATE_STATUS PostUpdate(float dt);
	bool CleanUp();

	void OnResize(int width, int height);

public:

	//App State
	std::vector<float> frameRateLog;

	
	//Window
	bool windowFullScreen;
	bool windowFullScreenDesktop;
	bool windowResizable;
	bool windowBorderless;
	int  brightness;


}; 
