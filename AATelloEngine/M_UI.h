#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"

class M_UI : public Module
{
public:
	M_UI(bool start_enabled = true);
	~M_UI();

	bool Init();
	UPDATE_STATUS PreUpdate(float dt) override;
	UPDATE_STATUS PostUpdate(float dt) override;
	bool CleanUp();

	void OnResize(int width, int height);

public:




}; 
