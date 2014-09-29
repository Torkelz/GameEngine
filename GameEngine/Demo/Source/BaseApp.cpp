#include "BaseApp.h"


const std::string BaseApp::m_GameTitle = "Particles";

BaseApp::BaseApp(void)
{
}


BaseApp::~BaseApp(void)
{
}

void BaseApp::init()
{
	m_MemUpdateDelay = 0.1f;
	m_TimeToNextMemUpdate = 0.f;

	m_Window.init(getGameTitle(), getWindowSize());
	m_NewWindowSize = m_Window.getSize();

	bool fullscreen = false;
	m_Graphics.initialize(m_Window.getHandle(), (int)m_Window.getSize().x, (int)m_Window.getSize().y, fullscreen);

	m_Window.registerCallback(WM_CLOSE, std::bind(&BaseApp::handleWindowClose, this, std::placeholders::_1,
		std::placeholders::_2, std::placeholders::_3));
	m_Window.registerCallback(WM_EXITSIZEMOVE, std::bind(&BaseApp::handleWindowExitSizeMove, this,
		std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
	m_Window.registerCallback(WM_SIZE, std::bind(&BaseApp::handleWindowSize, this, std::placeholders::_1,
		std::placeholders::_2, std::placeholders::_3));
}

void BaseApp::run()
{
	m_ShouldQuit = false;

	resetTimer();

	while (!m_ShouldQuit)
	{
		//m_InputQueue.onFrame();
		m_Window.pollMessages();
		updateTimer();
		/*handleInput();

		updateLogic();

		render();*/

		updateDebugInfo();
	}
}

void BaseApp::shutdown()
{

}

bool BaseApp::handleWindowClose( WPARAM /*p_WParam*/, LPARAM /*p_LParam*/, LRESULT& p_Result )
{
	m_ShouldQuit = true;
	p_Result = 0;
	return true;
}

bool BaseApp::handleWindowExitSizeMove( WPARAM /*p_WParam*/, LPARAM p_LParam, LRESULT& p_Result )
{
	m_Window.setSize(m_NewWindowSize);				

	p_Result = 0;
	return true;
}

bool BaseApp::handleWindowSize( WPARAM p_WParam, LPARAM p_LParam, LRESULT& p_Result )
{
	m_NewWindowSize = DirectX::XMFLOAT2(LOWORD(p_LParam),HIWORD(p_LParam));

	switch(p_WParam)
	{
	case SIZE_MAXIMIZED:
		{
			m_Window.setSize(m_NewWindowSize);
			m_Window.setIsMaximized(true);
			p_Result = 0;
			return true;
		}
	case SIZE_MAXHIDE:{return false;}
	case SIZE_MAXSHOW:{return false;}
	case SIZE_MINIMIZED:{return false;}
	case SIZE_RESTORED:
		{
			if(m_Window.getIsMaximized())
			{
				m_Window.setIsMaximized(false);
				m_Window.setSize(m_NewWindowSize);
				p_Result = 0;
				return true;
			}
			return false;
		}
	default:
		return false;
	}
}

void BaseApp::updateDebugInfo()
{
	m_TimeToNextMemUpdate -= m_DeltaTime;
	if (m_TimeToNextMemUpdate > 0.f)
	{
		return;
	}

	m_TimeToNextMemUpdate = m_MemUpdateDelay;

	std::string speed = "DeltaTime: " + std::to_string(m_DeltaTime) + " FPS: " + std::to_string(1.0f/m_DeltaTime);

	m_Window.setTitle(getGameTitle() + " | " + speed);
}

void BaseApp::resetTimer()
{
	__int64 cntsPerSec = 0;
	QueryPerformanceFrequency((LARGE_INTEGER*)&cntsPerSec);
	m_SecsPerCnt = 1.0f / (float)cntsPerSec;

	m_PrevTimeStamp = 0;
	m_CurrTimeStamp = 0;
	QueryPerformanceCounter((LARGE_INTEGER*)&m_CurrTimeStamp);
	m_CurrTimeStamp--;
}

void BaseApp::updateTimer()
{
	m_PrevTimeStamp = m_CurrTimeStamp;
	QueryPerformanceCounter((LARGE_INTEGER*)&m_CurrTimeStamp);
	m_DeltaTime = ((m_CurrTimeStamp - m_PrevTimeStamp) * m_SecsPerCnt);// / 10.0f; // To debug the game at low refreshrate.
	static const float maxDeltaTime = 1.f / 24.f; // Up from 5.f. Animations start behaving wierd if frame rate drops below 24. 
	if (m_DeltaTime > maxDeltaTime)
	{
		m_DeltaTime = maxDeltaTime;
	}
}

std::string BaseApp::getGameTitle() const
{
	return m_GameTitle;
}

DirectX::XMFLOAT2 BaseApp::getWindowSize() const
{
	// TODO: Read from user option

	const static DirectX::XMFLOAT2 size = DirectX::XMFLOAT2(1280, 720);
	return size;
}