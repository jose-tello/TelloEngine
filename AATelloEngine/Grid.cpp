#include "Grid.h"

#include "Glew/include/glew.h"
#include <gl/GL.h>
#include <gl/GLU.h>

Grid::Grid()
{
}


Grid::~Grid()
{
}


void Grid::Draw() const
{
	glLineWidth(1.0f);
	glColor3f(0.7f, 0.7f, 0.7f);

	glBegin(GL_LINES);

	float d = 120.0f;

	for (float i = -d; i <= d; i += 4.0f)
	{
		glVertex3f(i,  -0.2f, -d);
		glVertex3f(i,  -0.2f, d);
		glVertex3f(-d, -0.2f, i);
		glVertex3f(d,  -0.2f, i);
	}

	glEnd();
}