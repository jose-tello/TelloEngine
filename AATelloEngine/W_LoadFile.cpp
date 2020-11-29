#include "W_LoadFile.h"

#include "Application.h"
#include "M_FileManager.h"
#include "M_Editor.h"
#include "M_Resources.h"

#include "imgui/imgui.h"

#include <vector>
#include <string>

W_LoadFile::W_LoadFile(bool open) : E_Window(E_WINDOW_TYPE::LOAD_FILE, open)
{
}


W_LoadFile::~W_LoadFile()
{
}


bool W_LoadFile::Draw()
{
	ImGui::Begin(("Load file"), &open);
	
	if (ImGui::Button("Update state"))
		App->resourceManager->UpdateAllAssets();

	DrawLoadWindow();
	ImGui::End();
	
	return true;
}


void W_LoadFile::DrawLoadWindow()
{
	ImGui::BeginChild("File bowser");
	DrawDirectory("/Library", "");
	ImGui::EndChild();
}


void W_LoadFile::DrawDirectory(const char* directory, const char* filterExtension)
{
	std::vector<std::string> files;
	std::vector<std::string> folderDirs;

	std::string dir(directory);
	dir += "/";

	App->fileManager->ExploreDirectory(dir.c_str(), files, folderDirs);

	int folderDirsCount = folderDirs.size();
	for (int i = 0; i < folderDirsCount; i++)
	{
		if (ImGui::TreeNodeEx((dir + folderDirs[i]).c_str(), 0, "%s/", folderDirs[i].c_str()))
		{
			DrawDirectory((dir + folderDirs[i]).c_str(), filterExtension);
			ImGui::TreePop();
		}
	}

	int filesCount = files.size();
	for (int i = 0; i < filesCount; i++)
	{
		std::string str = files[i];

		str.substr(str.find_last_of(".") + 1);

		if (ImGui::TreeNodeEx(str.c_str(), ImGuiTreeNodeFlags_Leaf))
		{
			if (ImGui::IsItemClicked() && ImGui::IsMouseDoubleClicked(0))
			{
				//TODO: Once resource manager is implemented, use this path to load the resource
				App->editor->AddLog("Test load: %s", files[i].c_str());
				int id = std::stoi(files[i]);

				App->resourceManager->WindowLoad(id, App->editor->GetFocusedGameObject());
			}

			ImGui::TreePop();
		}
	}
}