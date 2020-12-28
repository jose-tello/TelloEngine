#ifndef __W_SHADER_EDITOR_H__
#define __W_SHADER_EDITOR_H__

#include "E_Window.h"

#include <string>

class TextEditor;

class W_ShaderEditor : public E_Window
{
public:
	W_ShaderEditor();
	~W_ShaderEditor() override;

	bool Draw() override;

	void OpenShaderCode(int resourceShader, const char* shaderName);

private: 
	void SaveShader();

private:
	int currentShader = 0;
	std::string currentShaderName;

	TextEditor* textEditor = nullptr;
};

#endif // !__W_SHADER_EDITOR__

