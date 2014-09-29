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

	DirectX::XMFLOAT4X4 m_ViewMatrix;
	DirectX::XMFLOAT4X4 m_ProjectionMatrix;
	DirectX::XMFLOAT3 m_Eye;

	unsigned int m_Numerator;
	unsigned int m_Denominator;
	char m_GraphicsCard[128];
	int m_GraphicsMemory;
	bool m_VSyncEnabled;
public:
	Graphics(void);
	~Graphics(void);

	void initialize(HWND p_Hwnd, int p_ScreenWidth, int p_ScreenHeight, bool p_Fullscreen);
	void shutdown();

private:
	HRESULT createDeviceAndSwapChain(HWND p_Hwnd, int p_ScreenWidth, int p_ScreenHeight, bool p_Fullscreen);
	HRESULT createRenderTargetView(void);
	HRESULT createDepthStencilBuffer(int p_ScreenWidth,	int p_ScreenHeight);
	HRESULT createDepthStencilState(void);
	HRESULT createDepthStencilView(void);
	HRESULT createRasterizerState(void);
};

