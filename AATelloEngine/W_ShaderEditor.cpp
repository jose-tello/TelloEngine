#include "W_ShaderEditor.h"

#include "ImGuiColorTextEdit/TextEditor.h"

#include "Application.h"
#include "M_Editor.h"
#include "M_Resources.h"
#include "M_FileManager.h"

#include "ShaderImporter.h"

W_ShaderEditor::W_ShaderEditor() : E_Window(E_WINDOW_TYPE::EDIT_SHADER, false),
	currentShader(0),
	textEditor(nullptr)
{
	textEditor = new TextEditor();
}


W_ShaderEditor::~W_ShaderEditor()
{
	currentShader = 0;
	delete textEditor;
	textEditor = nullptr;
}


bool W_ShaderEditor::Draw()
{
	ImGui::Begin("Shader editor", &open);
	
	ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), currentShaderName.c_str());
	ImGui::SameLine();
	ImGui::Spacing();
	ImGui::SameLine();

	if (ImGui::Button("Save"))
		SaveShader();
	
	textEditor->Render("Shader editor");

	ImGui::End();

	return true;
}


void W_ShaderEditor::OpenShaderCode(int resourceShader, const char* shaderName)
{
	char* fileBuffer = nullptr;
	textEditor->SelectAll();
	textEditor->Delete();

	std::string path(SHADER_LIBRARY);
	path.append(std::to_string(resourceShader));

	unsigned int size = App->fileManager->Load(path.c_str(), &fileBuffer);

	if (size > 0)
	{
		std::string code(fileBuffer);
		code = code.substr(0, code.find_last_of("}") + 1);

		textEditor->InsertText(code.c_str());
		currentShader = resourceShader;
		currentShaderName = shaderName;
		open = true;
	}
	else
		App->editor->AddLog("[WARNING], shader not loaded correctlly from: %s", path.c_str());

	delete[] fileBuffer;
	fileBuffer = nullptr;
}


void W_ShaderEditor::SaveShader()
{
	ShaderImporter::SaveAsAsset(currentShader, textEditor->GetText().c_str());
}