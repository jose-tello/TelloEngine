#include "Globals.h"
#include "Application.h"
#include "M_Camera3D.h"
#include "M_Input.h"
#include "M_Window.h"
#include "M_Scene.h"
#include "M_Renderer3D.h"

#include "M_Editor.h"
#include "W_CameraView.h"

#include "GameObject.h"
#include "C_Camera.h"
#include "C_Aberration.h"

#include "SDL\include\SDL_scancode.h"
#include "SDL\include\SDL_mouse.h"

#include "MathGeoLib/src/MathGeoLib.h"

M_Camera3D::M_Camera3D(bool start_enabled) : Module(start_enabled),
	drawClickRay(false),
	debugFrustumCull(false),
	drawAABB(true),
	deformedX(0.0f),
	deformedY(0.0f),
	deformedZ(0.0f),

	camera(nullptr)
{
}


M_Camera3D::~M_Camera3D()
{
	PopAberrations();
}


bool M_Camera3D::Start()
{
	camera = new C_Camera(true);
	camera->frustum.SetPos(float3(0, 0, -10));

	return true;
}


// -----------------------------------------------------------------
UPDATE_STATUS M_Camera3D::Update(float dt)
{
	float mouseX, mouseY;
	App->editor->GetCameraWindowSize(camera->GetWindow(), camWidth, camHeight, mouseX, mouseY);

	if (App->editor->IsWindowHovered(E_WINDOW_TYPE::SCENE_CAMERA))
	{
		if (App->input->GetKey(SDL_SCANCODE_F) == KEY_STATE::KEY_DOWN)
		{
			float3 pos;
			if (App->editor->GetFocusedGameObjectPos(pos.x, pos.y, pos.z) == true)
				camera->LookAt(pos);
		}

		else if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_STATE::KEY_REPEAT)
		{
			MoveCamera(dt);
			RotateCamera();
		}

		else if (App->input->GetMouseButton(SDL_BUTTON_MIDDLE) == KEY_STATE::KEY_REPEAT)
			MoveCameraSideways(dt);

		else if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_STATE::KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_LALT) == KEY_STATE::KEY_REPEAT)
		{
			float3 pos;
			if (App->editor->GetFocusedGameObjectPos(pos.x, pos.y, pos.z) == true)
				OrbitCamera(pos, dt);
		}

		int weelMotion = App->input->GetMouseZ();
		if (weelMotion != 0)
			ZoomCamera(weelMotion, dt);
	}

	CheckCameraInsideAberration();
	PopAberrations();
	return UPDATE_STATUS::UPDATE_CONTINUE;
}


// -----------------------------------------------------------------
float4x4 M_Camera3D::GetViewMatrix()
{
	return camera->GetViewMat();
}


float4x4 M_Camera3D::GetProjectionMatrix()
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


void M_Camera3D::ClickSelect()
{
	int width, height;

	float mouseX /*= App->input->GetMouseX()*/;
	float mouseY /*= App->input->GetMouseY()*/;

	App->editor->GetCameraWindowSize(camera->GetWindow(), width, height, mouseX, mouseY);

	mouseX = mouseX / width;
	mouseY = mouseY / height;

	mouseX = (mouseX - 0.5) / 0.5;
	mouseY = (mouseY - 0.5) / 0.5;

	LineSegment ray = camera->frustum.UnProjectLineSegment(mouseX, -mouseY);


	App->scene->TestRayCollision(ray);

	float3 nearPoint = ray.GetPoint(0);
	float3 farPoint = ray.GetPoint(50);

	float nearP[3] = { nearPoint.x, nearPoint.y, nearPoint.z };
	float farP[3] = { farPoint.x, farPoint.y, farPoint.z };
	App->renderer3D->SetCameraRay(nearP, farP);
}


void M_Camera3D::PushAberration(C_Aberration* aberration)
{
	aberrationVector.push_back(aberration);
}


void M_Camera3D::PopAberrations()
{
	aberrationVector.clear();
}


float M_Camera3D::GetNearPlaneDst() const
{
	return camera->GetNearPlaneDst();
}


void M_Camera3D::SetNearPlaneDst(float dst)
{
	camera->SetNearPlaneDst(dst);
}


float M_Camera3D::GetFarPlaneDst() const
{
	return camera->GetFarPlaneDst();
}


void M_Camera3D::SetFarPlaneDst(float dst)
{
	camera->SetFarPlaneDst(dst);
}


float M_Camera3D::GetVerticalFov() const
{
	return camera->GetVerticalFov();
}


void M_Camera3D::SetVerticalFov(float fov)
{
	camera->SetVerticalFov(fov);
}


float M_Camera3D::GetAspectRatio() const
{
	return camera->GetAspectRatio();
}


void M_Camera3D::SetAspectRatio(float aspRatio)
{
	camera->SetAspectRatio(aspRatio);
}


int M_Camera3D::GetWidth() const
{
	return camWidth;
}


int M_Camera3D::GetHeight() const
{
	return camHeight;
}


void M_Camera3D::CheckCameraInsideAberration()
{
	int aberrationCount = aberrationVector.size();

	for (int i = 0; i < aberrationCount; ++i)
	{
		std::vector<float> vertexArray;
		aberrationVector[i]->GetVertexArray(vertexArray);

		AABB aabb;
		aabb.SetNegativeInfinity();
		aabb.Enclose((float3*)&vertexArray[0], vertexArray.size() / 3);

		float4x4 iTrans = aberrationVector[i]->GetOwner()->transform.GetMatTransform();
		iTrans.Inverse();

		float4 point = float4(GetPosition(), 1.0);

		point = iTrans * point;
		float3 transFormedPoin = float3(point.x, point.y, point.z);

		if (aabb.Contains(transFormedPoin))
		{
			App->renderer3D->PushCameraInsideAberration(i);
			deformedX = aberrationVector[i]->GetDeformationX();
			deformedY = aberrationVector[i]->GetDeformationY();
			deformedZ = aberrationVector[i]->GetDeformationZ();

			return;
		}
	}

	deformedX = 0.0f;
	deformedY = 0.0f;
	deformedZ = 0.0f;
}


void M_Camera3D::MoveCamera(float dt)
{
	float3 direction(0, 0, 0);

	float speed = CAMERA_SPEED * dt;
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_STATE::KEY_REPEAT)
		speed *= 2;

	float3 Z = camera->frustum.Front();

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_STATE::KEY_REPEAT) direction += Z;
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_STATE::KEY_REPEAT) direction -= Z;

	float3 X = camera->frustum.WorldRight();

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_STATE::KEY_REPEAT) direction -= X;
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_STATE::KEY_REPEAT) direction += X;

	if (direction.IsZero() == false)
	{
		direction.Normalize();
		direction.x *= 1.0 + deformedX;
		direction.y *= 1.0 + deformedY;
		direction.z *= 1.0 + deformedZ;

		direction *= speed;

		float3 pos = camera->frustum.Pos();
		camera->frustum.SetPos(pos + direction);
	}
	
}


void M_Camera3D::RotateCamera()
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

	float speed = MOUSE_WEEL_SPEED * dt;

	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_STATE::KEY_REPEAT)
		speed *= 0.2f;

	if (weelMotion > 0)
		newPos += Z * speed;
	
	else
		newPos -= Z * speed;

	float3 position = camera->frustum.Pos();
	position += newPos;
	camera->frustum.SetPos(position);
}


void M_Camera3D::MoveCameraSideways(float dt)
{
	float3 newPos(0, 0, 0);
	float3 X = camera->frustum.WorldRight();
	float3 Y = camera->frustum.Up();

	int dx = -App->input->GetMouseXMotion();
	int dy = -App->input->GetMouseYMotion();

	float speed = CAMERA_SPEED * dt;

	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_STATE::KEY_REPEAT)
		speed *= 0.05f;

	newPos += dx * X * speed;
	newPos -= dy * Y * speed;
	
	float3 position = camera->frustum.Pos();
	position += newPos;
	camera->frustum.SetPos(position);
}


void M_Camera3D::OrbitCamera(float3& reference, float dt)
{
	int dx = -App->input->GetMouseXMotion();
	int dy = -App->input->GetMouseYMotion();

	float deltaX = (float)dx * MOUSE_ORBIT_SENSITIVITY * dt;
	float deltaY = (float)dy * MOUSE_ORBIT_SENSITIVITY * dt;

	float3 vector = camera->frustum.Pos() - reference;

	Quat quat_y(camera->frustum.Up(), deltaX);
	Quat quat_x(camera->frustum.WorldRight(), deltaY);

	vector = quat_x.Transform(vector);
	vector = quat_y.Transform(vector);

	camera->frustum.SetPos(vector + reference);
	camera->LookAt(reference);
}