#ifndef __E_WINDOW_H__
#define __E_WINDOW_H__

enum class E_WINDOW_TYPE : int
{
	WIN_ERROR = -1,
	DOCKING_WINDOW,
	APPLICATION_STATE,
	CONSOLE,
	GAME_OBJECTS,
	INSPECTOR,
	LOAD_FILE,
	EDIT_SHADER,
	SCENE_CAMERA,
	CAMERA_VIEW,
	MAX
};

class E_Window
{
public:
	E_Window(E_WINDOW_TYPE type, bool open = true);
	virtual ~E_Window();

	virtual bool Start();
	virtual bool Update();
	virtual bool Draw();

public:
	E_WINDOW_TYPE type;
	bool open;
};


#endif//__E_WINDOW_H__