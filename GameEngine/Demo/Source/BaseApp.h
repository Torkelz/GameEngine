#pragma once
#include "Window.h"
#include "Render.h"
#include "Input\Input.h"

class BaseApp
{
private:
	static const std::string m_GameTitle;

	Input m_InputQueue;

	bool m_ShouldQuit;
	float m_SecsPerCnt;
	int m_PrevTimeStamp;
	int m_CurrTimeStamp;
	float m_MemUpdateDelay;
	float m_TimeToNextMemUpdate;
	float m_DeltaTime;

	DirectX::XMFLOAT2 m_NewWindowSize;

	Window m_Window;
	Render m_Render;
public:
	BaseApp(void);
	~BaseApp(void);

	void init();
	void run();
	void shutdown();

private:
	bool handleWindowClose(WPARAM /*p_WParam*/, LPARAM /*p_LParam*/, LRESULT& p_Result);
	bool handleWindowExitSizeMove(WPARAM /*p_WParam*/, LPARAM p_LParam, LRESULT& p_Result);
	bool handleWindowSize(WPARAM p_WParam, LPARAM p_LParam, LRESULT& p_Result);
	
	void updateDebugInfo();
	void resetTimer();
	void updateTimer();
	void handleInput();
	std::string getGameTitle() const;

	DirectX::XMFLOAT2 getWindowSize() const;
};

