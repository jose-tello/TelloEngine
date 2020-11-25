#include "W_CameraView.h"

#include "Application.h"
#include "M_Renderer3D.h"
#include "M_Window.h"

#include "GameObject.h"
#include "C_Camera.h"

#include "imgui/imgui.h"

#include <string>

W_CameraView::W_CameraView(bool active, C_Camera* camera, E_WINDOW_TYPE type) : E_Window(type, active),
	windowWidth(0),
	windowHeight(0),
	windowPosX(0),
	windowPosY(0),
	mousePosX(0),
	mousePosY(0),

	frameBuffer(0),
	textureBuffer(0),
	depthBuffer(0),

	camera(camera)
{
}


W_CameraView::~W_CameraView()
{
	App->renderer3D->DeleteBuffers(frameBuffer, textureBuffer, depthBuffer);
	
	frameBuffer = 0;
	textureBuffer = 0;
	depthBuffer = 0;
}


bool W_CameraView::Draw()
{
	std::string name;
	GameObject* cam = camera->GetOwner();

	if (cam != nullptr)
		name = cam->GetName();
	else
		name = "No GO";

	ImGui::Begin(name.c_str(), &open);
	ImGui::BeginChild("Game render");

	ImVec2 size = ImGui::GetWindowSize();
	ImVec2 winPos = ImGui::GetWindowPos();
	ImVec2 mousePos = ImGui::GetMousePos();
	windowPosX = winPos.x;
	windowPosY = winPos.y;
	mousePosX = mousePos.x;
	mousePosY = mousePos.y;

	if (size.x != windowWidth || size.y != windowHeight)
		OnResize(size.x, size.y);

	App->renderer3D->DrawScene(frameBuffer, camera);

	ImGui::Image((ImTextureID)textureBuffer, ImVec2(windowWidth, windowHeight), ImVec2(0, 1), ImVec2(1, 0));
	ImGui::EndChild();
	ImGui::End();

	return true;
}


void W_CameraView::GetWindowMeasures(int& width, int& height) const
{
	width = windowWidth;
	height = windowHeight;
}


void W_CameraView::GetWindowPos(float& x, float& y) const
{
	x = windowPosX;
	y = windowPosY;
}


void W_CameraView::GetMousePosInWindow(float& mouseX, float& mouseY) const
{
	mouseX = mousePosX - windowPosX;
	mouseY = mousePosY - windowPosY;
}


void W_CameraView::OnResize(int x, int y)
{
	windowWidth = x;
	windowHeight = y;

	App->renderer3D->OnResize(windowWidth, windowHeight, camera);
	App->renderer3D->DeleteBuffers(frameBuffer, textureBuffer, depthBuffer);
	App->renderer3D->GenerateFrameBuffer(windowWidth, windowHeight, frameBuffer, textureBuffer, depthBuffer);
}