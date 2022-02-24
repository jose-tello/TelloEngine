#include "W_AppState.h"

#include "Application.h"
#include "M_Input.h"
#include "M_Window.h"
#include "M_Renderer3D.h"
#include "M_Camera3D.h"
#include "M_Resources.h"

#include "Resource.h"

#include "TimeManager.h"

#include "imgui/imgui.h"
#include "SDL\include\SDL.h"

W_AppState::W_AppState(bool open) :	E_Window(E_WINDOW_TYPE::APPLICATION_STATE, open),

	//Window
	winFullScreen(false),
	winFullScreenDesktop(false),
	winResizable(true),
	winBorderless(true),

	brightness(100),

	//Render
	depthTestEnabled(true),
	cullFaceEnabled(true),
	lightingEnabled(true),
	colorMatEnabled(true),
	texture2DEnabled(true),
	fillModeEnabled(true),
	wireframeModeEnabled(false),
	vsync(true),
	rasterizationRender(false),

	//Cpu
	cpuCores(0),
	maxRamMemory(0),

	has3DNow(false),
	hasAVX(false),
	hasAVX2(false),
	hasAltiVec(false),
	hasMMX(false),
	hasRDTSC(false),
	hasSSE(false),
	hasSSE2(false),
	hasSSE3(false),
	hasSSE41(false),
	hasSSE42(false)
{

}


W_AppState::~W_AppState()
{
	frameRateLog.clear();
	inputsLog.clear();
}

bool W_AppState::Start()
{
	cpuCores = SDL_GetCPUCount();
	maxRamMemory = SDL_GetSystemRAM();

	has3DNow = SDL_Has3DNow();
	hasAVX = SDL_HasAVX();
	hasAVX2 = SDL_HasAVX2();
	hasAltiVec = SDL_HasAltiVec();
	hasMMX = SDL_HasMMX();
	hasRDTSC = SDL_HasRDTSC();
	hasSSE = SDL_HasSSE();
	hasSSE2 = SDL_HasSSE2();
	hasSSE3 = SDL_HasSSE3();
	hasSSE41 = SDL_HasSSE41();
	hasSSE42 = SDL_HasSSE42();

	return true;
}


bool W_AppState::Update()
{
	UpdateChApplicationState();
	UpdateChInput();

	return true;
}


bool W_AppState::Draw()
{
	ImGui::Begin("Application state", &open);

	DrawChApplicationState();
	DrawChWindow();
	DrawChRenderOptions();
	DrawCameraOptions();
	DrawReferenceCounting();
	DrawChInput();
	DrawChHardware();

	ImGui::End();

	return true;
}


void W_AppState::UpdateChApplicationState()
{
	frameRateLog.push_back(ImGui::GetIO().Framerate);

	if (frameRateLog.size() > MAX_LOG_SIZE)
		frameRateLog.erase(frameRateLog.begin());
}


void W_AppState::UpdateChInput()
{
	App->input->ReportKeyState(inputsLog);

	if (inputsLog.size() > MAX_LOG_SIZE / 4)
	{
		int elementsToErase = inputsLog.size() - MAX_LOG_SIZE / 4;
		for (int i = 0; i < elementsToErase; i++)
		{
			inputsLog.erase(inputsLog.begin());
		}
	}
}


void W_AppState::DrawChApplicationState()
{
	if (ImGui::CollapsingHeader("Application state"))
	{
		ImGui::PlotHistogram("##framerate", &frameRateLog.front(), frameRateLog.size(), 0, "FrameRate", 0.0f, 100.0f, ImVec2(310, 100));
	
		TimeManager* time = App->GetTimeManager();
		ImGui::Text("Time since start: %f", time->GetTimeSinceStart());
		ImGui::Text("Time since play: %f", time->GetTimeSincePlay());
		ImGui::Text("Game dt: %f", time->GetDt());
	}
}


void W_AppState::DrawChWindow()
{
	if (ImGui::CollapsingHeader("Window"))
	{
		ImGui::Checkbox("Full screen", &winFullScreen);
		ImGui::SameLine();
		ImGui::Checkbox("Full screen desktop", &winFullScreenDesktop);
		ImGui::Checkbox("Resizable", &winResizable);
		ImGui::SameLine();
		ImGui::Checkbox("Borderless", &winBorderless);


		ImGui::SliderInt("Brightness", &brightness, 1, 100);

		App->window->SetWindowFullScreen(winFullScreen);
		App->window->SetWindowFullScreenDesktop(winFullScreenDesktop);
		App->window->SetWindowResizable(winResizable);
		App->window->SetWindowBorderless(winBorderless);

		float bright = brightness * 0.01;
		App->window->SetWindowBrightness(bright);
	}
}


void W_AppState::DrawChRenderOptions()
{
	if (ImGui::CollapsingHeader("Render options"))
	{
		ImGui::Checkbox("Depth test", &depthTestEnabled);
		ImGui::Checkbox("Backface cull", &cullFaceEnabled);
		ImGui::Checkbox("Lightning", &lightingEnabled);
		ImGui::Checkbox("Color material", &colorMatEnabled);
		ImGui::Checkbox("Texture 2D", &texture2DEnabled);
		ImGui::Checkbox("Fill mode", &fillModeEnabled);
		ImGui::Checkbox("Wireframe mode", &wireframeModeEnabled);
		ImGui::Checkbox("Vsync", &vsync);
		ImGui::Checkbox("Rasterization render", &rasterizationRender);


		App->renderer3D->SetDepthTestEnabled(depthTestEnabled);
		App->renderer3D->SetCullFaceEnabled(cullFaceEnabled);
		App->renderer3D->SetLightingEnabled(lightingEnabled);
		App->renderer3D->SetColorMatEnabled(colorMatEnabled);
		App->renderer3D->SetTexture2DEnabled(texture2DEnabled);
		App->renderer3D->SetFillMode(fillModeEnabled);
		App->renderer3D->SetVsync(vsync);
		App->renderer3D->SetRasterization(rasterizationRender);
	}
}


void W_AppState::DrawCameraOptions()
{
	if (ImGui::CollapsingHeader("Camera"))
	{
		float aspectRatio = App->camera->GetAspectRatio();
		float verticalFov = App->camera->GetVerticalFov();
		float farDst = App->camera->GetFarPlaneDst();
		float nearDst = App->camera->GetNearPlaneDst();

		if (ImGui::InputFloat("Aspect ratio", &aspectRatio, ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue))
			App->camera->SetAspectRatio(aspectRatio);

		if (ImGui::InputFloat("Vertical fov", &verticalFov, ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue))
			App->camera->SetVerticalFov(verticalFov);

		if (ImGui::InputFloat("Far plane distance", &farDst, ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue))
			App->camera->SetFarPlaneDst(farDst);

		if (ImGui::InputFloat("Near plane distance", &nearDst, ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue))
			App->camera->SetNearPlaneDst(nearDst);

		ImGui::Checkbox("Debug mouse pick ray", &App->camera->drawClickRay);
		ImGui::Checkbox("Debug frustum cull", &App->camera->debugFrustumCull);
		ImGui::Checkbox("Draw aabb and frustum boxes", &App->camera->drawAABB);
	}
}


void W_AppState::DrawReferenceCounting()
{
	if (ImGui::CollapsingHeader("Reference counting"))
	{
		std::vector<Resource*> meshes, materials, textures, models, shaders;
		App->resourceManager->GetAllResources(meshes, materials, textures, models, shaders);

		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Meshes");

		int meshesCount = meshes.size();
		for (int i = 0; i < meshesCount; i++)
		{
			if (meshes[i]->GetReferenceCount() > 0)
				ImGui::Text("%i: %i", meshes[i]->GetUid(), meshes[i]->GetReferenceCount());
		}

		ImGui::Separator();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Materials");

		int materialsCount = materials.size();
		for (int i = 0; i < materialsCount; i++)
		{
			if (materials[i]->GetReferenceCount() > 0)
				ImGui::Text("%i: %i", materials[i]->GetUid(), materials[i]->GetReferenceCount());
		}

		ImGui::Separator();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Textures");

		int texturesCount = textures.size();
		for (int i = 0; i < texturesCount; i++)
		{
			if (textures[i]->GetReferenceCount() > 0)
				ImGui::Text("%i: %i", textures[i]->GetUid(), textures[i]->GetReferenceCount());
		}

		ImGui::Separator();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Shaders");

		int shadersCount = shaders.size();
		for (int i = 0; i < shadersCount; i++)
		{
			if (shaders[i]->GetReferenceCount() > 0)
				ImGui::Text("%i: %i", shaders[i]->GetUid(), shaders[i]->GetReferenceCount());
		}
	}
}


void W_AppState::DrawChInput()
{
	if (ImGui::CollapsingHeader("Input"))
	{
		ImGui::Text("Mouse position: "); ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%i x,	%i y", App->input->GetMouseX(), App->input->GetMouseY());

		ImGui::Text("Mouse motion: "); ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%i x,	%i y", App->input->GetMouseXMotion(), App->input->GetMouseYMotion());

		ImGui::Separator();
		ImGui::Text("Key and mouse inputs: \n");
		ImGui::NewLine();

		if (inputsLog.empty() == false)
		{
			int logSize = inputsLog.size();

			for (int i = 0; i < logSize; i++)
			{
				ImGui::Text(inputsLog[i].c_str());
			}
		}
	}
}


void W_AppState::DrawChHardware()
{
	if (ImGui::CollapsingHeader("Hardware"))
	{
		ImGui::Text("CPU cache used: "); ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%i MB", SDL_GetCPUCacheLineSize());
		ImGui::Text("CPU cores: "); ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%i", cpuCores);

		ImGui::Text("Caps: ");

		if (has3DNow == true)
			ImGui::BulletText("3DNow");

		if (hasAVX == true)
			ImGui::BulletText("AVX");

		if (hasAVX2 == true)
			ImGui::BulletText("AVX2");

		if (hasAltiVec == true)
			ImGui::BulletText("AltiVec");

		if (hasMMX == true)
			ImGui::BulletText("MMX");

		if (hasRDTSC == true)
			ImGui::BulletText("RDTSC");

		if (hasSSE == true)
			ImGui::BulletText("SSE");

		if (hasSSE2 == true)
			ImGui::BulletText("SSE2");

		if (hasSSE3 == true)
			ImGui::BulletText("SSE3");

		if (hasSSE41 == true)
			ImGui::BulletText("SSE41");

		if (hasSSE42 == true)
			ImGui::BulletText("SSE42");
		
		ImGui::Spacing();

		ImGui::Separator();
		ImGui::Text("Libraries used:");

		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "MathGeoLib v1.5");
		ImGui::Spacing();

		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "ImGui");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), ImGui::GetVersion());
		ImGui::Spacing();

		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "SDL");
		ImGui::SameLine();
		SDL_version compiled;
		SDL_VERSION(&compiled);
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%d.%d.%d", compiled.major, compiled.minor, compiled.patch);

		ImGui::Spacing();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "OpenGL v. 3.0");
		ImGui::Spacing();

		ImGui::TextColored(ImVec4(1.f, 1.f, 0.f, 1.f), "DevIl v1.7.8");
		ImGui::Spacing();

		ImGui::TextColored(ImVec4(1.f, 1.f, 0.f, 1.f), "Assimp");
		ImGui::Spacing();
	}
}