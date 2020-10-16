#include "Globals.h"
#include "Application.h"
#include "M_Renderer3D.h"
#include "M_Window.h"
#include "M_Camera3D.h"
#include "M_Editor.h"

#include "Primitive.h"
#include "Mesh.h"

#include "Glew/include/glew.h"
#pragma comment(lib,"Glew/libx86/glew32.lib")

#include "SDL\include\SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#include "glmath.h"


#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */

M_Renderer3D::M_Renderer3D(bool start_enabled) : Module(start_enabled),
	context(),
	
	frameBuffer(0),
	textureBuffer(0),
	depthBuffer(0),

	cube(nullptr),
	piramid(nullptr),
	sphere(nullptr),

	depthTestEnabled(true),
	cullFaceEnabled(true),
	lightingEnabled(true),
	colorMatEnabled(true),
	texture2DEnabled(true),
	wireframeModeEnabled(false)
{
}

// Destructor
M_Renderer3D::~M_Renderer3D()
{}


// Called before render is available
bool M_Renderer3D::Init()
{
	App->editor->AddLog("Log: Creating 3D Renderer context");
	bool ret = true;
	
	//Create context
	context = SDL_GL_CreateContext(App->window->window);
	if(context == NULL)
	{
		App->editor->AddLog("ERROR: OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		GLenum glewError = glewInit();
		
		if (glewError != GLEW_OK)
		{
			App->editor->AddLog("ERROR: Error initializing GLEW! %s\n", glewGetErrorString(glewError));
		}
	}

	if(ret == true)
	{
		//Use Vsync
		if(VSYNC && SDL_GL_SetSwapInterval(1) < 0)
			App->editor->AddLog("ERROR: Unable to set VSync! SDL Error: %s\n", SDL_GetError());

		//Initialize Projection Matrix
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		//Check for error
		GLenum error = glGetError();
		if(error != GL_NO_ERROR)
		{
			App->editor->AddLog("ERROR: Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}

		//Initialize Modelview Matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			App->editor->AddLog("ERROR: Error initializing OpenGL! %s\n", gluErrorString(error));
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
			App->editor->AddLog("ERROR: Error initializing OpenGL! %s\n", gluErrorString(error));
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
	vec3 pos2(0, 6, -15);
	vec3 pos3(2, 0, -5);
	vec3 pos4(5, 2, 5);
	vec3 rotation(1, 0, 0);

	cube = new Cube(pos, 45.f, rotation, 0.f, 1.f);
	sphere = new Sphere(3, 10, 16, pos2, 0.f, rotation);
	piramid = new Piramid(pos3, 0, rotation, 1.f, 0.f, 0.f);
	cilinder = new Cilinder(12, 4, 4, pos4, 0.f, rotation, 1.f, 0.5f, 1.f);

	piramid->SetEscale(vec3(4, 4, 4));

	return ret;
}

// PreUpdate: clear buffer
UPDATE_STATUS M_Renderer3D::PreUpdate(float dt)
{
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(App->camera->GetViewMatrix());


	return UPDATE_STATUS::UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
UPDATE_STATUS M_Renderer3D::PostUpdate(float dt)
{
	DrawSceneTexture();
	App->editor->Draw();
	SDL_GL_SwapWindow(App->window->window);

	return UPDATE_STATUS::UPDATE_CONTINUE;
}

// Called before quitting
bool M_Renderer3D::CleanUp()
{
	App->editor->AddLog("Log: Destroying 3D Renderer");

	SDL_GL_DeleteContext(context);

	glDeleteFramebuffers(1, &frameBuffer);
	frameBuffer = 0;
	glDeleteFramebuffers(1, &textureBuffer);
	textureBuffer = 0;
	glDeleteFramebuffers(1, &depthBuffer);
	depthBuffer = 0;

	//TODO: Someday i will have to quit the primitives here, but now they are fine
	delete cube;
	delete piramid;
	delete sphere;
	delete cilinder;

	cube = nullptr;
	piramid = nullptr;
	sphere = nullptr;
	cilinder = nullptr;

	return true;
}


void M_Renderer3D::OnResize(float width, float height)
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	projectionMatrix = perspective(60.0f, width / height, 0.125f, 512.0f);
	glLoadMatrixf(&projectionMatrix);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	GenerateFrameBuffer(width, height);
}


void M_Renderer3D::SetDepthTestEnabled(bool enable)
{
	if (depthTestEnabled != enable)
	{
		if (enable == true)
			glEnable(GL_DEPTH_TEST);

		else
			glDisable(GL_DEPTH_TEST);

		depthTestEnabled = enable;
	}
}


void M_Renderer3D::SetCullFaceEnabled(bool enable)
{
	if (cullFaceEnabled != enable)
	{
		if (enable == true)
			glEnable(GL_CULL_FACE);

		else
			glDisable(GL_CULL_FACE);

		cullFaceEnabled = enable;
	}
}


void M_Renderer3D::SetLightingEnabled(bool enable)
{
	if (lightingEnabled != enable)
	{
		if (enable == true)
			glEnable(GL_LIGHTING);

		else
			glDisable(GL_LIGHTING);
	
		lightingEnabled = enable;
	}
}


void M_Renderer3D::SetColorMatEnabled(bool enable)
{
	if (colorMatEnabled != enable)
	{
		if (enable == true)
			glEnable(GL_COLOR_MATERIAL);

		else 
			glDisable(GL_COLOR_MATERIAL);

		colorMatEnabled = enable;
	}
}


void M_Renderer3D::SetTexture2DEnabled(bool enable)
{
	if (texture2DEnabled != enable)
	{
		if (enable == true)
			glEnable(GL_TEXTURE_2D);

		else
			glDisable(GL_TEXTURE_2D);

		texture2DEnabled = enable;
	}
}


void M_Renderer3D::SetWireframeMode(bool enable)
{
	if (wireframeModeEnabled != enable)
	{
		if (enable == true)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		wireframeModeEnabled = enable;
	}
}


void M_Renderer3D::GenerateFrameBuffer(float width, float height)
{
	glGenFramebuffers(1, &frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

	glGenTextures(1, &textureBuffer);
	glBindTexture(GL_TEXTURE_2D, textureBuffer);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenRenderbuffers(1, &depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);

	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureBuffer, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}


void M_Renderer3D::DrawSceneTexture()
{
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);

	if (wireframeModeEnabled == true)
		glLineWidth(3.0f);

	cube->GetMesh()->Draw();
	sphere->GetMesh()->Draw();
	piramid->GetMesh()->Draw();
	cilinder->GetMesh()->Draw();

	Plane plane(0, 1, 0, 1);
	plane.Draw();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}