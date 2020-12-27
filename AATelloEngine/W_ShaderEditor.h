#ifndef __W_SHADER_EDITOR_H__
#define __W_SHADER_EDITOR_H__

#include "E_Window.h"

class TextEditor;

class W_ShaderEditor : public E_Window
{
public:
	W_ShaderEditor();
	~W_ShaderEditor() override;

	bool Draw() override;

	void OpenShaderCode(int resourceShader);

private:
	int currentShader = 0;

	TextEditor* textEditor = nullptr;
};

#endif // !__W_SHADER_EDITOR__

