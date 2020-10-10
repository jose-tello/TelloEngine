#ifndef __PRIMITIVE_VERTEX_H__
#define __PRIMITIVE_VERTEX_H__

//Cube
float cubeVertexArray[] = {
	0.f, 0.f, 0.f,
	10.f, 0.f, 0.f,
	10.f, 0.f, -10.f,
	0, 0, -10.f,

	0.f, 10.f, 0.f,
	10.f, 10.f, 0.f,
	10.f, 10.f, -10.f,
	0.f, 10.f, -10.f
};


unsigned int cubeIndexArray[] = {
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




#endif //__PRIMITIVE_VERTEX_H__