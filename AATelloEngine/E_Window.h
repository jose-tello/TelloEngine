#ifndef __E_WINDOW_H__
#define __E_WINDOW_H__

enum class E_WINDOW_TYPE : int
{
	WIN_ERROR = -1,
	APPLICATION_STATE,
	CONSOLE,
	SCENE,
	MAX
};

class E_Window
{
public:
	E_Window(bool open = true);
	~E_Window();

	virtual bool Start();
	virtual bool Update();
	virtual bool Draw();

public:
	bool open;
};


#endif//__E_WINDOW_H__