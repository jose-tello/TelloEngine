#ifndef __PRIMITIVE_VERTEX_H__
#define __PRIMITIVE_VERTEX_H__

//Cube
static float cubeVertexArray[] = {
	0.f, 0.f, 0.f,
	1.f, 0.f, 0.f,
	1.f, 0.f, -1.f,
	0, 0, -1.f,

	0.f, 1.f, 0.f,
	1.f, 1.f, 0.f,
	1.f, 1.f, -1.f,
	0.f, 1.f, -1.f
};


static unsigned int cubeIndexArray[] = {
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


//Piramid

static float piramidVertexArray[] = {

	0.f, 0.f, 0.f,
	1.f, 0.f, 0.f,
	1.f, 0.f, 1.f,
	0.f, 0.f, 1.f,

	.5f, 1.f, .5f
};


static unsigned int piramidIndexArray[] = {
	4, 1, 0,
	4, 2, 1,
	4, 3, 2,
	4, 0, 3,

	3, 0, 1,
	1, 2, 3
};

#endif //__PRIMITIVE_VERTEX_H__