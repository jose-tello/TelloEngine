#include "E_Window.h"

E_Window::E_Window(E_WINDOW_TYPE type, bool open) : 
	type(type),
	open(open)
{
}


E_Window::~E_Window()
{
}


bool E_Window::Start()
{
	return true;
}


bool E_Window::Update()
{
	return true;
}

bool E_Window::Draw()
{
	return true;
}