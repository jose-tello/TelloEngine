#ifndef __E_CONSOLE_H__
#define __E_CONSOLE_H__

#include "E_Window.h"

#include "imgui/imgui.h"

#include <string>

#define INPUT_BUFFER_SIZE 256


class W_Console : public E_Window
{
public:
	W_Console(bool open = true);
	~W_Console();

	bool Draw() override;

	void AddLog(char* log);

private:
	void AddCommandLog(const char* fmt, ...) IM_FMTARGS(2);
	void ClearLog();

	void ExecuteCommand(const char* command);

	static int TextEditCallbackStub(ImGuiInputTextCallbackData* data);
	static void  Strtrim(char* s);
	int TextEditCallback(ImGuiInputTextCallbackData* data);


private:
	char inputBuffer[INPUT_BUFFER_SIZE];
	ImVector<char*> items;
	ImVector<const char*> commands;

	ImGuiTextFilter filter;
	bool autoScroll;
	bool scrollToBottom;
};


#endif//__E_CONSOLE_H__

