#include "Globals.h"
#include "Application.h"
#include "M_Renderer3D.h"
#include "M_Window.h"
#include "M_Camera3D.h"
#include "M_Console.h"

#include "Primitive.h"

#include "Glew/include/glew.h"
#pragma comment(lib,"Glew/libx86/glew32.lib")

#include "SDL\include\SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#include "glmath.h"


#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */

ModuleRenderer3D::ModuleRenderer3D(bool start_enabled) : Module(start_enabled), context()
{
}

// Destructor
ModuleRenderer3D::~ModuleRenderer3D()
{}


float vertexArray[] = {
	0.f, 0.f, 0.f,
	10.f, 0.f, 0.f,
	10.f, 0.f, -10.f,
	0, 0, -10.f,

	0.f, 10.f, 0.f,
	10.f, 10.f, 0.f,
	10.f, 10.f, -10.f,
	0.f, 10.f, -10.f
};


uint indexArray[] = {
	4, 0, 1,
	1, 5, 4,

	4, 7, 3,
	3, 0, 4,

	2, 3, 7, 
	7, 6, 2,

	7, 4, 5,
	5, 6, 7,

	5, 1, 2,
	2, 6, 5,

	0, 3, 2,
	2, 1, 0
};

// Called before render is available
bool ModuleRenderer3D::Init()
{
	App->console->AddLog("Log: Creating 3D Renderer context");
	bool ret = true;
	
	//Create context
	context = SDL_GL_CreateContext(App->window->window);
	if(context == NULL)
	{
		App->console->AddLog("ERROR: OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		GLenum glewError = glewInit();
		
		if (glewError != GLEW_OK)
		{
			App->console->AddLog("ERROR: Error initializing GLEW! %s\n", glewGetErrorString(glewError));
		}
	}

	if(ret == true)
	{
		//Use Vsync
		if(VSYNC && SDL_GL_SetSwapInterval(1) < 0)
			App->console->AddLog("ERROR: Unable to set VSync! SDL Error: %s\n", SDL_GetError());

		//Initialize Projection Matrix
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		//Check for error
		GLenum error = glGetError();
		if(error != GL_NO_ERROR)
		{
			App->console->AddLog("ERROR: Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}

		//Initialize Modelview Matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			App->console->AddLog("ERROR: Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}
		
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glClearDepth(1.0f);
		
		//Initialize clear color
		glClearColor(0.f, 0.f, 0.f, 1.f);

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			App->console->AddLog("ERROR: Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}
		
		
		GLfloat MaterialAmbient[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

		GLfloat MaterialDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);
		
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glEnable(GL_LIGHTING);
		glEnable(GL_COLOR_MATERIAL);
	}

	// Projection matrix for
	OnResize(SCREEN_WIDTH, SCREEN_HEIGHT);

	
	vec3 pos(-5, 3, -5);
	vec3 pos2(0, 0, -15);
	vec3 pos3(2, 0, -5);
	vec3 rotation(1, 0, 0);

	cube = new Cube(pos, 45.f, rotation, 0.f, 1.f);
	sphere = new Sphere(1, 10, 16, pos2, 0.f, rotation);
	piramid = new Piramid(pos3, 0, rotation);

	piramid->SetEscale(vec3(4, 4, 4));

	return ret;
}

// PreUpdate: clear buffer
UPDATE_STATUS ModuleRenderer3D::PreUpdate(float dt)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(App->camera->GetViewMatrix());

	cube->Draw();
	sphere->Draw();
	piramid->Draw();

	Plane plane(0, 1, 0, 1);
	plane.Draw();

	

	return UPDATE_STATUS::UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
UPDATE_STATUS ModuleRenderer3D::PostUpdate(float dt)
{
	//SDL_GL_SwapWindow(App->window->window);
	return UPDATE_STATUS::UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRenderer3D::CleanUp()
{
	App->console->AddLog("Log: Destroying 3D Renderer");

	SDL_GL_DeleteContext(context);

	return true;
}


void ModuleRenderer3D::OnResize(int width, int height)
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	ProjectionMatrix = perspective(60.0f, (float)width / (float)height, 0.125f, 512.0f);
	glLoadMatrixf(&ProjectionMatrix);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
