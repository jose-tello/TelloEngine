#ifndef __M_CONSOLE_H__
#define __M_CONSOLE_H__

#include "Globals.h"
#include "Module.h"

#include "imgui/imgui.h"

#include <vector>

#define INPUT_BUFFER_SIZE 256

class M_Console : public Module
{
public:
	M_Console();
	~M_Console();

	UPDATE_STATUS PreUpdate(float dt);
	UPDATE_STATUS PostUpdate(float dt);

	void AddLog(const char* fmt, ...) IM_FMTARGS(2);

private:
	
	void ClearLog();
	
	void ExecuteCommand(const char* command);

	static int TextEditCallbackStub(ImGuiInputTextCallbackData* data);
	static void  Strtrim(char* s) { char* str_end = s + strlen(s); while (str_end > s && str_end[-1] == ' ') str_end--; *str_end = 0; }
	int TextEditCallback(ImGuiInputTextCallbackData* data);

	void Draw(const char* title, bool* open);

private:
	char inputBuffer[INPUT_BUFFER_SIZE];
	ImVector<char*> items;
	ImVector<const char*> commands;

	ImGuiTextFilter filter;
	bool autoScroll;
	bool scrollToBottom;

	bool consoleOpen;
};



#endif //__M_CONSOLE_H__