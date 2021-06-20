#include "E_Window.h"

E_Window::E_Window(E_WINDOW_TYPE type, bool open) : 
	type(type),
	open(open),
	hovered(false),
	focused(false)
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


bool E_Window::IsHovered() const
{
	return hovered;
}

bool E_Window::IsFocused() const
{
	return focused;
}
