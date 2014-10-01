#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
class Graphics;

class Render
{
private:
	Graphics *m_Graphics;

public:
	Render();
	~Render();
	void initialize(HWND p_Hwnd, int p_ScreenWidth, int p_ScreenHeight, bool p_Fullscreen);
	void shutdown();
};

