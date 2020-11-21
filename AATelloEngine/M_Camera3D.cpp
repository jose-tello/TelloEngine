#include "Globals.h"
#include "Application.h"
#include "M_Camera3D.h"
#include "M_Input.h"

#include "M_Editor.h"
#include "W_Scene.h"

#include "GameObject.h"
#include "C_Transform.h"
#include "C_Camera.h"

#include "SDL\include\SDL_scancode.h"
#include "SDL\include\SDL_mouse.h"

#include "MathGeoLib/src/MathGeoLib.h"

M_Camera3D::M_Camera3D(bool start_enabled) : Module(start_enabled),
	camera(nullptr)
{
}


M_Camera3D::~M_Camera3D()
{
}


bool M_Camera3D::Start()
{
	camera = new C_Camera();
	camera->frustum.SetPos(float3(0, 0, -10));

	return true;
}


// -----------------------------------------------------------------
UPDATE_STATUS M_Camera3D::Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_STATE::KEY_DOWN)
	{
		GameObject* obj = App->editor->GetFocusedGameObject();
		if (obj != nullptr)
		{
			float3 pos;
			obj->transform.GetPos(pos.x, pos.y, pos.z);
			camera->LookAt(pos);
		}
	}

	else if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_STATE::KEY_REPEAT)
	{
		MoveCamera(dt);
		RotateCamera(dt);
	}

	else if (App->input->GetMouseButton(SDL_BUTTON_MIDDLE) == KEY_STATE::KEY_REPEAT)
		MoveCameraSideways(dt);

	else if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_STATE::KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_LALT) == KEY_STATE::KEY_REPEAT)
	{
		GameObject* obj = App->editor->GetFocusedGameObject();
		if (obj != nullptr)
		{
			float3 pos;
			obj->transform.GetPos(pos.x, pos.y, pos.z);
			camera->LookAt(pos);
		}

		MoveCamera(dt);
		RotateCamera(dt);
	}

	int weelMotion = App->input->GetMouseZ();
	if (weelMotion != 0)
		ZoomCamera(weelMotion, dt);


	return UPDATE_STATUS::UPDATE_CONTINUE;
}


// -----------------------------------------------------------------
float* M_Camera3D::GetViewMatrix()
{
	return camera->GetViewMat();
}


float* M_Camera3D::GetProjectionMatrix()
{
	return camera->GetProjectionMat();
}


float3 M_Camera3D::GetPosition()
{
	return camera->frustum.Pos();
}


void M_Camera3D::Resize(float width, float height)
{
	camera->SetProjectionMat(camera->GetVerticalFov(), width / height);
}


void M_Camera3D::MoveCamera(float dt)
{
	float3 newPos(0, 0, 0);

	float speed = CAMERA_SPEED * dt;
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_STATE::KEY_REPEAT)
		speed *= 2;

	float3 Z = camera->frustum.Front();

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_STATE::KEY_REPEAT) newPos += Z * speed;
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_STATE::KEY_REPEAT) newPos -= Z * speed;

	float3 X = camera->frustum.WorldRight();

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_STATE::KEY_REPEAT) newPos -= X * speed;
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_STATE::KEY_REPEAT) newPos += X * speed;

	float3 pos = camera->frustum.Pos();
	newPos += pos;
	camera->frustum.SetPos(newPos);	
}


void M_Camera3D::RotateCamera(float dt)
{
	int dx = -App->input->GetMouseXMotion();
	int dy = -App->input->GetMouseYMotion();

	if (dx != 0)
	{
		float deltaX = (float)dx * MOUSE_SENSITIVITY;

		Quat incrementX(float3::unitY, deltaX * DEGTORAD);
		incrementX.Normalize();

		camera->frustum.SetFront(incrementX.ToFloat3x3() * camera->frustum.Front());
		camera->frustum.SetUp(incrementX.ToFloat3x3() * camera->frustum.Up());
	}

	if (dy != 0)
	{
		float deltaY = (float)dy * MOUSE_SENSITIVITY;

		Quat incrementY(camera->frustum.WorldRight(), deltaY * DEGTORAD);
		incrementY.Normalize();

		camera->frustum.SetUp(incrementY.ToFloat3x3() * camera->frustum.Up());
		camera->frustum.SetFront(incrementY.ToFloat3x3() * camera->frustum.Front());

		if (camera->frustum.Up().y < 0.0f)
		{
			camera->frustum.SetFront(float3(0.0f, camera->frustum.Front().y > 0.0f ? 1.0f : -1.0f, 0.0f));
			camera->frustum.SetUp(float3(-camera->frustum.Front()).Cross(camera->frustum.WorldRight()));
		}
	}
}


void M_Camera3D::ZoomCamera(int weelMotion, float dt)
{
	float3 newPos(0, 0, 0);
	float3 Z = camera->frustum.Front();

	if (weelMotion > 0)
		newPos += Z * MOUSE_WEEL_SPEED * dt;
	
	else
		newPos -= Z * MOUSE_WEEL_SPEED * dt;

	float3 position = camera->frustum.Pos();
	position += newPos;
	camera->frustum.SetPos(position);
}


void M_Camera3D::MoveCameraSideways(float dt)
{
	float3 newPos(0, 0, 0);
	float3 X = camera->frustum.WorldRight();

	int dx = -App->input->GetMouseXMotion();
	int dy = -App->input->GetMouseYMotion();

	newPos += dx * X * CAMERA_SPEED * 0.25f * dt;
	newPos.y -= dy * CAMERA_SPEED * 0.25f * dt;
	
	float3 position = camera->frustum.Pos();
	position += newPos;
	camera->frustum.SetPos(position);
}