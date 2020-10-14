#include "E_Console.h"
#include "Application.h"

E_Console::E_Console(bool open) :
	E_Window(open),
	autoScroll(true),
	scrollToBottom(false)
{
	memset(inputBuffer, 0, sizeof(inputBuffer));

	commands.push_back("help");
	commands.push_back("clear");
	commands.push_back("classify");
}

E_Console::~E_Console()
{
	ClearLog();

	items.clear();
	commands.clear();
}


bool E_Console::Draw()
{
	ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);
	ImGui::Begin("console", &open);
	
	if (ImGui::BeginPopupContextItem())
	{
		if (ImGui::MenuItem("Close Console"))
			open = false;
		ImGui::EndPopup();
	}

	ImGui::TextWrapped("Enter 'help' too see commands.");

	ImGui::Separator();

	// Options menu
	if (ImGui::BeginPopup("Options"))
	{
		ImGui::Checkbox("Auto-scroll", &autoScroll);
		ImGui::EndPopup();
	}

	// Options, Filter
	if (ImGui::Button("Options"))
		ImGui::OpenPopup("Options");
	ImGui::SameLine();
	filter.Draw("Filter (\"incl,-excl\") (\"error\")", 180);
	ImGui::Separator();

	// Reserve enough left-over height for 1 separator + 1 input text
	const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
	ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), false, ImGuiWindowFlags_HorizontalScrollbar);
	if (ImGui::BeginPopupContextWindow())
	{
		if (ImGui::Selectable("Clear")) ClearLog();
		ImGui::EndPopup();
	}

	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1)); // Tighten spacing

	for (int i = 0; i < items.size(); i++)
	{
		const char* item = items[i];
		if (!filter.PassFilter(item))
			continue;

		ImVec4 color;
		bool has_color = false;
		if (strstr(item, "[error]")) { color = ImVec4(1.0f, 0.4f, 0.4f, 1.0f); has_color = true; }
		else if (strncmp(item, "# ", 2) == 0) { color = ImVec4(1.0f, 0.8f, 0.6f, 1.0f); has_color = true; }
		if (has_color)
			ImGui::PushStyleColor(ImGuiCol_Text, color);
		ImGui::TextUnformatted(item);
		if (has_color)
			ImGui::PopStyleColor();
	}


	if (scrollToBottom || (autoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY()))
		ImGui::SetScrollHereY(1.0f);
	scrollToBottom = false;

	ImGui::PopStyleVar();
	ImGui::EndChild();
	ImGui::Separator();

	// Command-line
	bool reclaim_focus = false;
	ImGuiInputTextFlags input_text_flags = ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackCompletion | ImGuiInputTextFlags_CallbackHistory;
	if (ImGui::InputText("Input", inputBuffer, IM_ARRAYSIZE(inputBuffer), input_text_flags, &TextEditCallbackStub, (void*)this))
	{
		char* s = inputBuffer;
		Strtrim(s);
		if (s[0])
			ExecuteCommand(s);
		strcpy(s, "");
		reclaim_focus = true;
	}

	// Auto-focus on window apparition
	ImGui::SetItemDefaultFocus();
	if (reclaim_focus)
		ImGui::SetKeyboardFocusHere(-1); // Auto focus previous widget

	ImGui::End();

	return true;
}


void E_Console::AddLog(char* log)
{
	items.push_back(strdup(log));
}


void E_Console::ClearLog()
{
	for (int i = 0; i < items.size(); i++)
		free(items[i]);
	items.clear();
}


void E_Console::AddCommandLog(const char* fmt, ...) IM_FMTARGS(2)
{
	char buf[1024];
	va_list args;
	va_start(args, fmt);
	vsnprintf(buf, IM_ARRAYSIZE(buf), fmt, args);
	buf[IM_ARRAYSIZE(buf) - 1] = 0;
	va_end(args);

	AddLog(buf);
}


void E_Console::ExecuteCommand(const char* command)
{
	AddCommandLog("# %s\n", command);

	if (stricmp(command, "clear") == 0)
	{
		ClearLog();
	}
	else if (stricmp(command, "help") == 0)
	{
		AddLog("Commands:");
		for (int i = 0; i < commands.size(); i++)
			AddCommandLog("- %s", commands[i]);
	}
	else
	{
		AddCommandLog("Unknown command: '%s'\n", command);
	}

	// On command input, we scroll to bottom even if AutoScroll==false
	scrollToBottom = true;
}


int E_Console::TextEditCallbackStub(ImGuiInputTextCallbackData* data)
{
	E_Console* console = (E_Console*)data->UserData;
	return console->TextEditCallback(data);
}


void E_Console::Strtrim(char* s) 
{ 
	char* str_end = s + strlen(s); 
	while (str_end > s && str_end[-1] == ' ') str_end--; *str_end = 0; 
}


int E_Console::TextEditCallback(ImGuiInputTextCallbackData* data)
{
	switch (data->EventFlag)
	{
	case ImGuiInputTextFlags_CallbackCompletion:
	{
		// Example of TEXT COMPLETION

		// Locate beginning of current word
		const char* word_end = data->Buf + data->CursorPos;
		const char* word_start = word_end;
		while (word_start > data->Buf)
		{
			const char c = word_start[-1];
			if (c == ' ' || c == '\t' || c == ',' || c == ';')
				break;
			word_start--;
		}

		// Build a list of candidates
		ImVector<const char*> candidates;
		for (int i = 0; i < commands.size(); i++)
			if (strnicmp(commands[i], word_start, (int)(word_end - word_start)) == 0)
				candidates.push_back(commands[i]);

		if (candidates.Size == 0)
		{
			// No match
			AddCommandLog("No match for \"%.*s\"!\n", (int)(word_end - word_start), word_start);
		}
		else if (candidates.Size == 1)
		{
			// Single match. Delete the beginning of the word and replace it entirely so we've got nice casing.
			data->DeleteChars((int)(word_start - data->Buf), (int)(word_end - word_start));
			data->InsertChars(data->CursorPos, candidates[0]);
			data->InsertChars(data->CursorPos, " ");
		}
		else
		{
			// Multiple matches. Complete as much as we can..
			// So inputing "C"+Tab will complete to "CL" then display "CLEAR" and "CLASSIFY" as matches.
			int match_len = (int)(word_end - word_start);
			for (;;)
			{
				int c = 0;
				bool all_candidates_matches = true;
				for (int i = 0; i < candidates.Size && all_candidates_matches; i++)
					if (i == 0)
						c = toupper(candidates[i][match_len]);
					else if (c == 0 || c != toupper(candidates[i][match_len]))
						all_candidates_matches = false;
				if (!all_candidates_matches)
					break;
				match_len++;
			}

			if (match_len > 0)
			{
				data->DeleteChars((int)(word_start - data->Buf), (int)(word_end - word_start));
				data->InsertChars(data->CursorPos, candidates[0], candidates[0] + match_len);
			}

			// List matches
			AddLog("Possible matches:\n");
			for (int i = 0; i < candidates.Size; i++)
				AddCommandLog("- %s\n", candidates[i]);
		}

		break;
	}
	}
	return 0;
}