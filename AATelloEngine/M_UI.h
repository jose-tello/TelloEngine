#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"

#include <vector>

#define MAX_LOG_SIZE 80
#define MAX_RESOLUTION_WIDTH 1920
#define MAX_RESOLUTION_HEIGHT 1080

class M_UI : public Module
{
public:
	M_UI(bool start_enabled = true);
	~M_UI();

	bool Init();
	UPDATE_STATUS PreUpdate(float dt);
	UPDATE_STATUS PostUpdate(float dt);
	bool CleanUp();

public:

	//App State
	std::vector<float> frameRateLog;

	
	//Window
	bool winFullScreen;
	bool winFullScreenDesktop;
	bool winResizable;
	bool winBorderless;
	int  brightness;

	int winWidth;
	int winHeight;
}; 
