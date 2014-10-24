#pragma once
#include "Window.h"
#include "Render.h"
#include "Level.h"
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

	Vector2 m_NewWindowSize;

	Window m_Window;
	Render m_Render;
	Level	m_Level;

	Vector3 m_CameraPosition;
	Vector3 m_CameraDirection;
	Vector3 m_CameraUp;
	float m_CameraSpeed;
	Res::ResourceManager *m_Resourcemanager;
public:
	BaseApp(void);
	~BaseApp(void);

	/**
	* Initialize all the necessary components.
	*/
	void init(void);

	/**
	* The programs main loop, eg. update and render.
	*/
	void run(void);

	/**
	* Shutdowns all the components initialized.
	*/
	void shutdown(void);

private:
	bool handleWindowClose(WPARAM /*p_WParam*/, LPARAM /*p_LParam*/, LRESULT& p_Result);
	bool handleWindowExitSizeMove(WPARAM /*p_WParam*/, LPARAM p_LParam, LRESULT& p_Result);
	bool handleWindowSize(WPARAM p_WParam, LPARAM p_LParam, LRESULT& p_Result);
	
	void updateDebugInfo(void);
	void resetTimer(void);
	void updateTimer(void);
	void handleInput(void);
	void movePlayerView(float p_Yaw, float p_Pitch);

	std::string getGameTitle(void) const;

	Vector2 getWindowSize(void) const;
};

