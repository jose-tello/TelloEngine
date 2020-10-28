#include "DrawPlane.h"

#include "Glew/include/glew.h"
#include <gl/GL.h>
#include <gl/GLU.h>

DrawPlane::DrawPlane()
{
}


DrawPlane::~DrawPlane()
{
}


void DrawPlane::Draw() const
{
	glLineWidth(1.0f);
	glColor3f(1, 1, 1);

	glBegin(GL_LINES);

	float d = 200.0f;

	for (float i = -d; i <= d; i += 1.0f)
	{
		glVertex3f(i, 0.0f, -d);
		glVertex3f(i, 0.0f, d);
		glVertex3f(-d, 0.0f, i);
		glVertex3f(d, 0.0f, i);
	}

	glEnd();
}