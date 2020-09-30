#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"

#define MAX_MOUSE_BUTTONS 5
#define MAX_KEYS 300

enum class KEY_STATE : int
{
	KEY_IDLE = 0,
	KEY_DOWN,
	KEY_REPEAT,
	KEY_UP
};


enum class EVENT_WINDOW //used to control mouse events
{
	WE_QUIT = 0,
	WE_HIDE,
	WE_SHOW,
	WE_COUNT
};


class ModuleInput : public Module
{
public:
	
	ModuleInput(Application* app, bool start_enabled = true);
	~ModuleInput();

	bool Init();
	bool PreUpdate(float dt);
	bool CleanUp();

	KEY_STATE GetKey(int id) const;

	KEY_STATE GetMouseButton(int id) const;

	iMPoint GetMousePosScreen() const;
	void GetMouseMotion(int& x, int& y);
	void GetScrollWheelMotion(int& x, int& y);

private:
	bool		windowEvents[(int)EVENT_WINDOW::WE_COUNT];
	KEY_STATE  keyboard[MAX_KEYS];
	KEY_STATE	mouseButtons[MAX_MOUSE_BUTTONS];
	int			mouseMotionX;
	int			mouseMotionY;
	int			mouseX;
	int			mouseY;
	int			mouseXRaw;
	int			mouseYRaw;

	int			mouseWheelMotionX;
	int			mouseWheelMotionY;
	
};