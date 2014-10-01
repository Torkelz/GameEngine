#include "Render.h"
#include "Graphics.h"
#include "Utilities.h"

Render::Render() :
	m_Graphics(nullptr)
{
}


Render::~Render()
{
}

void Render::initialize(HWND p_Hwnd, int p_ScreenWidth, int p_ScreenHeight, bool p_Fullscreen)
{
	if (m_Graphics)
	{
		m_Graphics->shutdown();
		SAFE_DELETE(m_Graphics);
	}
	m_Graphics = new Graphics();
	m_Graphics->initialize(p_Hwnd, p_ScreenWidth, p_ScreenHeight, p_Fullscreen);
}

void Render::shutdown()
{
	if (m_Graphics)
	{
		m_Graphics->shutdown();
		SAFE_DELETE(m_Graphics);
	}
}