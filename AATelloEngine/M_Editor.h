#ifndef __M_EDITOR_H__
#define __M_EDITOR_H__

#include "Module.h"

#include <vector>

#define MAX_LOG_SIZE 80
#define MAX_RESOLUTION_WIDTH 1920
#define MAX_RESOLUTION_HEIGHT 1080

class E_Window;
enum class E_WINDOW_TYPE;

struct GameObject;
class C_Camera;

class M_Editor : public Module
{
public:
	M_Editor(bool startEnabled = true);
	~M_Editor() override;

 	bool Init() override;
	bool Start() override;
	
	UPDATE_STATUS Update(float dt) override;
	void Draw();

	bool CleanUp() override;
	
	void AddLog(const char* fmt, ...);

	void OpenWindow(int open);
	void DeleteWindow(E_Window* toDelete);

	E_Window* AddSceneWindow(C_Camera* camera);
	
	std::vector<E_Window*> GetWindowsVector();
	E_Window* GetWindow(int win);
	void GetSceneWindowSize(E_Window*, int& x, int& y, float& mouseX, float& mouseY);

	GameObject* GetFocusedGameObject() const;
	bool GetFocusedGameObjectPos(float& x, float& y, float& z) const;

private:
	std::vector<E_Window*> windowsVec;
}; 

#endif // !__M_EDITOR_H__