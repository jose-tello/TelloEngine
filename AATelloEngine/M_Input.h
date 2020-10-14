#pragma once
#include "Module.h"
#include "Globals.h"

#include <string>
#define MAX_KEYS 300
#define MAX_MOUSE_BUTTONS 5

enum class KEY_STATE : int
{
	KEY_IDLE = 0,
	KEY_DOWN,
	KEY_REPEAT,
	KEY_UP
};

class M_Input : public Module
{
public:
	
	M_Input(bool start_enabled = true);
	~M_Input();

	bool Init() override;
	UPDATE_STATUS PreUpdate(float dt) override;
	UPDATE_STATUS Update(float dt) override;
	bool CleanUp() override;

	KEY_STATE GetKey(int id) const;
	KEY_STATE GetMouseButton(int id) const;
	void ReportKeyState(std::vector<std::string>&) const;
	
	int GetMouseX() const;
	int GetMouseY() const;
	int GetMouseZ() const;

	int GetMouseXMotion() const;
	int GetMouseYMotion() const;

private:
	KEY_STATE keyboard[MAX_KEYS];
	KEY_STATE mouseButtons[MAX_MOUSE_BUTTONS];

	int mouse_x;
	int mouse_y;
	int mouse_z;
	int mouse_x_motion;
	int mouse_y_motion;
	//int mouse_z_motion;
};