#pragma once

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")

#include <d3d11.h>
#include <dxgi.h>
#include <DirectXMath.h>

class Graphics
{
private:
	ID3D11Device *m_Device;
	ID3D11DeviceContext *m_DeviceContext;

	IDXGISwapChain *m_SwapChain;
	ID3D11RenderTargetView *m_RenderTargetView;
	ID3D11SamplerState *m_Sampler;

	ID3D11RasterizerState *m_RasterState;

	ID3D11Texture2D *m_DepthStencilBuffer;
	ID3D11DepthStencilState	*m_DepthStencilState;
	ID3D11DepthStencilView *m_DepthStencilView;

	unsigned int m_Numerator;
	unsigned int m_Denominator;
	char m_GraphicsCard[128];
	int m_GraphicsMemory;
	bool m_VSyncEnabled;
public:
	Graphics(void);
	~Graphics(void);
	
	/**
	* Initializes all the required DirectX components.
	* @param p_Hwnd, a handle to a window.
	* @param p_ScreenWidth, the windows width.
	* @param p_ScreenHeight, the windows height.
	* @param p_Fullscreen, true if it should be in fullscreen.

	*/
	void initialize(HWND p_Hwnd, int p_ScreenWidth, int p_ScreenHeight, bool p_Fullscreen);
	
	/**
	* Cleans upp all DirectX components.
	*/
	void shutdown(void);

	/**
	* Prepares the graphics for a new frame, e.g. clearing rendertarget.
	* @param color[4], the clearcolor for the backbuffer.
	*/
	void Graphics::begin(float color[4]);

	/**
	* Swaps the rendertarget with the backbuffer.
	* @param p_Dt, the programs delta time in seconds.
	*/
	void end(void);

	/**
	* Get the ID3D11DeviceContext pointer.
	* @return, returns the pointer. Could be nullptr.
	*/
	ID3D11DeviceContext *getDeviceContext(void);

	/**
	* Get the ID3D11Device pointer.
	* @return, returns the pointer. Could be nullptr.
	*/
	ID3D11Device *getDevice(void);

private:
	HRESULT createDeviceAndSwapChain(HWND p_Hwnd, int p_ScreenWidth, int p_ScreenHeight, bool p_Fullscreen);
	HRESULT createRenderTargetView(void);
	HRESULT createDepthStencilBuffer(int p_ScreenWidth,	int p_ScreenHeight);
	HRESULT createDepthStencilState(void);
	HRESULT createDepthStencilView(void);
	HRESULT createRasterizerState(void);
};

