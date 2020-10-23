#include "Globals.h"
#include "Application.h"
#include "M_Camera3D.h"
#include "M_Input.h"


#include "SDL\include\SDL.h"

M_Camera3D::M_Camera3D(bool start_enabled) : Module(start_enabled),
	X(1.f, 0.f, 0.f),
	Y(0.f, 1.f, 0.f),
	Z(0.f, 0.f, 1.f),

	reference(0.f, 0.f, 0.f),
	position(0.f, 0.f, 0.f),

	viewMatrix(),
	viewMatrixInverse()
{
	CalculateViewMatrix();
}


M_Camera3D::~M_Camera3D()
{}


// -----------------------------------------------------------------
UPDATE_STATUS M_Camera3D::Update(float dt)
{
	if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_STATE::KEY_REPEAT)
		MoveCamera(dt);
	

	else if (App->input->GetMouseButton(SDL_BUTTON_MIDDLE) == KEY_STATE::KEY_REPEAT)
		MoveCameraSideways(dt);


	int weelMotion = App->input->GetMouseZ();
	if (weelMotion != 0)
		ZoomCamera(weelMotion, dt);

	// Recalculate matrix -------------
	CalculateViewMatrix();

	return UPDATE_STATUS::UPDATE_CONTINUE;
}

// -----------------------------------------------------------------
void M_Camera3D::Look(const vec3& position, const vec3& reference, bool rotateAroundReference)
{
	this->position = position;
	this->reference = reference;

	Z = normalize(position - reference);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);

	if (!rotateAroundReference)
	{
		this->reference = this->position;
		this->position += Z * 0.05f;
	}

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
void M_Camera3D::LookAt(const vec3& spot)
{
	reference = spot;

	Z = normalize(position - reference);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);

	CalculateViewMatrix();
}


// -----------------------------------------------------------------
void M_Camera3D::Move(const vec3& movement)
{
	position += movement;
	reference += movement;

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
float* M_Camera3D::GetViewMatrix()
{
	return &viewMatrix;
}

// -----------------------------------------------------------------
void M_Camera3D::CalculateViewMatrix()
{
	viewMatrix = mat4x4(X.x, Y.x, Z.x, 0.0f, X.y, Y.y, Z.y, 0.0f, X.z, Y.z, Z.z, 0.0f, -dot(X, position), -dot(Y, position), -dot(Z, position), 1.0f);
	viewMatrixInverse = inverse(viewMatrix);
}


void M_Camera3D::MoveCamera(float dt)
{
	vec3 newPos(0, 0, 0);
	float speed = CAMERA_SPEED * dt;
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_STATE::KEY_REPEAT)
		speed *= 2;

	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_STATE::KEY_REPEAT) newPos.y += speed;
	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_STATE::KEY_REPEAT) newPos.y -= speed;

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_STATE::KEY_REPEAT) newPos -= Z * speed;
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_STATE::KEY_REPEAT) newPos += Z * speed;


	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_STATE::KEY_REPEAT) newPos -= X * speed;
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_STATE::KEY_REPEAT) newPos += X * speed;

	position += newPos;
	reference += newPos;

	int dx = -App->input->GetMouseXMotion();
	int dy = -App->input->GetMouseYMotion();

	position -= reference;

	if (dx != 0)
	{
		float deltaX = (float)dx * MOUSE_SENSITIVITY;

		X = rotate(X, deltaX, vec3(0.0f, 1.0f, 0.0f));
		Y = rotate(Y, deltaX, vec3(0.0f, 1.0f, 0.0f));
		Z = rotate(Z, deltaX, vec3(0.0f, 1.0f, 0.0f));
	}

	if (dy != 0)
	{
		float deltaY = (float)dy * MOUSE_SENSITIVITY;

		Y = rotate(Y, deltaY, X);
		Z = rotate(Z, deltaY, X);

		if (Y.y < 0.0f)
		{
			Z = vec3(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
			Y = cross(Z, X);
		}
	}

	position = reference + Z * length(position);
}


void M_Camera3D::ZoomCamera(int weelMotion, float dt)
{
	vec3 newPos(0, 0, 0);

	if (weelMotion > 0)
		newPos -= Z * MOUSE_WEEL_SPEED * dt;
	
	else
		newPos += Z * MOUSE_WEEL_SPEED * dt;

	position += newPos;
	reference += newPos;
}


void M_Camera3D::MoveCameraSideways(float dt)
{
	vec3 newPos(0, 0, 0);

	int dx = -App->input->GetMouseXMotion();
	int dy = -App->input->GetMouseYMotion();

	newPos -= dx * X * CAMERA_SPEED * 0.25f * dt;
	newPos.y += dy * CAMERA_SPEED * 0.25f * dt;
	

	position += newPos;
	reference += newPos;
}


void M_Camera3D::MoveCameraOrbital(float dt)
{
	
}

