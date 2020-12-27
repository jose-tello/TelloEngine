#include "W_ShaderEditor.h"

#include "ImGuiColorTextEdit/TextEditor.h"

#include "Application.h"
#include "M_Editor.h"
#include "M_Resources.h"
#include "M_FileManager.h"

W_ShaderEditor::W_ShaderEditor() : E_Window(E_WINDOW_TYPE::EDIT_SHADER, true),
	currentShader(0),
	textEditor(nullptr)
{
	textEditor = new TextEditor();
}


W_ShaderEditor::~W_ShaderEditor()
{
	delete textEditor;
	textEditor = nullptr;
}


bool W_ShaderEditor::Draw()
{
	ImGui::Begin("Shader editor", &open);
	textEditor->Render("Shader editor");

	ImGui::End();

	return true;
}


void W_ShaderEditor::OpenShaderCode(int resourceShader)
{
	char* fileBuffer = nullptr;

	std::string path(SHADER_LIBRARY);
	path.append(std::to_string(resourceShader));

	unsigned int size = App->fileManager->Load(path.c_str(), &fileBuffer);

	if (size > 0)
	{
		std::string code(fileBuffer);
		code = code.substr(0, code.find_last_of("}") + 1);

		textEditor->InsertText(code.c_str());
	}
	else
		App->editor->AddLog("[WARNING], shader not loaded correctlly from: %s", path.c_str());

	delete[] fileBuffer;
	fileBuffer = nullptr;
}