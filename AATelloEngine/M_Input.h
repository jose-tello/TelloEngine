#ifndef __M_INPUT_H__
#define __M_INPUT_H__

#include "Module.h"

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
	
	M_Input(bool startEnabled = true);
	~M_Input();

	bool Init() override;
	UPDATE_STATUS PreUpdate(float dt) override;
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

	int mouseX;
	int mouseY;
	int mouseZ;
	int xMouseMotion;
	int yMouseMotion;
};

#endif // __M_INPUT_H__