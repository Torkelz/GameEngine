#include "Graphics.h"
#include "Utilities.h"
#include "UserExceptions.h"
#include "WrapperFactory.h"


Graphics::Graphics(void)
{
	m_Device = nullptr;
	m_DeviceContext = nullptr;

	m_SwapChain = nullptr;
	m_RenderTargetView = nullptr;
	m_Sampler = nullptr;

	m_RasterState = nullptr;

	m_DepthStencilBuffer = nullptr;
	m_DepthStencilState = nullptr;
	m_DepthStencilView = nullptr;
	m_VSyncEnabled = true;
}


Graphics::~Graphics(void)
{
}

void Graphics::initialize(HWND p_Hwnd, int p_ScreenWidth, int p_ScreenHeight, bool p_Fullscreen)
{
	HRESULT result;
	IDXGIFactory *factory = nullptr;
	IDXGIAdapter *adapter = nullptr;
	IDXGIOutput *adapterOutput = nullptr;

	unsigned int numModes;
	unsigned int stringLength;

	DXGI_MODE_DESC *displayModeList = nullptr;
	DXGI_ADAPTER_DESC adapterDesc;
	int error;

	result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
	if(FAILED(result))
	{
		throw GraphicsException("Error when creating DirectX graphics interface factory", __LINE__,__FILE__);
	}

	result = factory->EnumAdapters(0, &adapter);
	if(FAILED(result))
	{
		throw GraphicsException("Error when creating an adapter for the primary graphics interface", __LINE__,__FILE__);
	}

	result = adapter->EnumOutputs(0, &adapterOutput);
	if(FAILED(result))
	{
		throw GraphicsException("Error when enumerating the primary adapter output", __LINE__,__FILE__);
	}

	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED,
		&numModes, NULL);
	if(FAILED(result))
	{
		throw GraphicsException("Error when getting the display modes", __LINE__,__FILE__);
	}

	displayModeList = new DXGI_MODE_DESC[numModes];
	if(!displayModeList)
	{
		throw GraphicsException("Error when creating display mode list", __LINE__,__FILE__);
	}

	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED,
		&numModes, displayModeList);
	if(FAILED(result))
	{
		throw GraphicsException("Error when writing the display mode list", __LINE__,__FILE__);
	}

	for(unsigned int i = 0; i < numModes; i++)
	{
		if(displayModeList[i].Width == (unsigned int)p_ScreenWidth)
		{
			if(displayModeList[i].Height == (unsigned int)p_ScreenHeight)
			{
				m_Numerator = displayModeList[i].RefreshRate.Numerator;
				m_Denominator = displayModeList[i].RefreshRate.Denominator;
			}
		}
	}

	result = adapter->GetDesc(&adapterDesc);
	if(FAILED(result))
	{
		throw GraphicsException("Error when getting the graphics card description", __LINE__,__FILE__);
	}

	m_GraphicsMemory = (int)(adapterDesc.DedicatedVideoMemory / (1024*1024));

	error = wcstombs_s(&stringLength, m_GraphicsCard, 128, adapterDesc.Description, 128);
	if(error != 0)
	{
		throw GraphicsException("Error when storing the graphics card name", __LINE__,__FILE__);
	}

	SAFE_DELETE_ARRAY(displayModeList);
	SAFE_RELEASE(adapterOutput);
	SAFE_RELEASE(adapter);
	SAFE_RELEASE(factory);

	result = createDeviceAndSwapChain(p_Hwnd, p_ScreenWidth, p_ScreenHeight, p_Fullscreen);
	if(FAILED(result))
	{
		throw GraphicsException("Error when creating the device and swap chain", __LINE__,__FILE__);
	}

	result = createRenderTargetView();
	if(FAILED(result))
	{
		throw GraphicsException("Error when creating render target view", __LINE__,__FILE__);
	}

	result = createDepthStencilBuffer(p_ScreenWidth, p_ScreenHeight);
	if(FAILED(result))
	{
		throw GraphicsException("Error when creating the depth stencil buffer", __LINE__,__FILE__);
	}

	result = createDepthStencilState();
	if(FAILED(result))
	{
		throw GraphicsException("Error when creating the depth stencil state", __LINE__,__FILE__);
	}

	m_DeviceContext->OMSetDepthStencilState(m_DepthStencilState, 1);

	result = createDepthStencilView();
	if(FAILED(result))
	{
		throw GraphicsException("Error when creating the depth stencil view", __LINE__,__FILE__);
	}

	m_DeviceContext->OMSetRenderTargets(1, &m_RenderTargetView, m_DepthStencilView);

	result = createRasterizerState();
	if(FAILED(result))
	{
		throw GraphicsException("Error when creating the rasterizer state", __LINE__,__FILE__);
	}

	D3D11_VIEWPORT viewport;
	viewport.Width = (float)p_ScreenWidth;
	viewport.Height = (float)p_ScreenHeight;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
	m_DeviceContext->RSSetViewports(1, &viewport);

	WrapperFactory::initialize(m_Device, m_DeviceContext);
}

void Graphics::shutdown()
{
	WrapperFactory::getInstance()->shutdown();
}

void Graphics::begin(float color[4])
{
	m_DeviceContext->ClearRenderTargetView(m_RenderTargetView, color);
	m_DeviceContext->ClearDepthStencilView(m_DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	m_DeviceContext->OMSetRenderTargets(1, &m_RenderTargetView, m_DepthStencilView);

	m_DeviceContext->RSSetState(m_RasterState);
	m_DeviceContext->OMSetDepthStencilState(m_DepthStencilState, 0);
}

void Graphics::end(void)
{
	if (m_VSyncEnabled)
	{
		// Lock to screen refresh rate.
		m_SwapChain->Present(1, 0);
	}
	else
	{
		// Present as fast as possible.
		m_SwapChain->Present(0, 0);
	}
}

HRESULT Graphics::createDeviceAndSwapChain(HWND p_Hwnd, int p_ScreenWidth, int p_ScreenHeight,
										   bool p_Fullscreen)
{
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	D3D_FEATURE_LEVEL featureLevel;

	//Initialize the swap chain description.
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	// Set to a single back buffer.
	swapChainDesc.BufferCount = 1;

	// Set the width and height of the back buffer.
	swapChainDesc.BufferDesc.Width = p_ScreenWidth;
	swapChainDesc.BufferDesc.Height = p_ScreenHeight;

	// Set regular 32-bit surface for the back buffer.
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	// Set the refresh rate of the back buffer.
	if(m_VSyncEnabled)
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = m_Numerator;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = m_Denominator;
	}
	else
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}

	// Set the usage of the back buffer.
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	// Set the handle for the window to render to.
	swapChainDesc.OutputWindow = p_Hwnd;

	// Turn multisampling off.
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	// Set to full screen or windowed mode.
	if(p_Fullscreen)
	{
		swapChainDesc.Windowed = false;
	}
	else
	{
		swapChainDesc.Windowed = true;
	}

	// Set the scan line ordering and scaling to unspecified.
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// Discard the back buffer contents after presenting.
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	// Don't set the advanced flags.
	swapChainDesc.Flags = 0;

	// Set the feature level to DirectX 11.
	featureLevel = D3D_FEATURE_LEVEL_11_0;

	return D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &featureLevel, 1, 
		D3D11_SDK_VERSION, &swapChainDesc, &m_SwapChain, &m_Device, NULL, &m_DeviceContext);
}

HRESULT Graphics::createRenderTargetView(void)
{
	HRESULT result;
	ID3D11Texture2D *backBufferPtr;

	//Get the pointer to the back buffer.
	result = m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);
	if(FAILED(result))
	{
		SAFE_RELEASE(backBufferPtr);

		return result;
	}

	D3D11_TEXTURE2D_DESC desc;
	backBufferPtr->GetDesc(&desc);
	//Create the render target view with the back buffer pointer.
	result = m_Device->CreateRenderTargetView(backBufferPtr, NULL, &m_RenderTargetView);
	SAFE_RELEASE(backBufferPtr);

	return result;
}

HRESULT Graphics::createDepthStencilBuffer(int p_ScreenWidth, int p_ScreenHeight)
{
	D3D11_TEXTURE2D_DESC depthBufferDesc;

	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));


	//Set up the description of the depth buffer.
	depthBufferDesc.Width = p_ScreenWidth;
	depthBufferDesc.Height = p_ScreenHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;//DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;
	
	//Create the texture for the depth buffer using the filled out description.
	return m_Device->CreateTexture2D(&depthBufferDesc, NULL, &m_DepthStencilBuffer);
}

HRESULT Graphics::createDepthStencilState(void)
{
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;

	//Initialize the description of the stencil state.
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	//Set up the description of the stencil state.
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	//Stencil operations if pixel is front-facing.
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing.
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	return m_Device->CreateDepthStencilState(&depthStencilDesc, &m_DepthStencilState);
}

HRESULT Graphics::createDepthStencilView(void)
{
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;

	//Initiailize the depth stencil view.
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	//Set up the depth stencil view description.
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	return m_Device->CreateDepthStencilView(m_DepthStencilBuffer, &depthStencilViewDesc, &m_DepthStencilView);
}

HRESULT Graphics::createRasterizerState(void)
{
	D3D11_RASTERIZER_DESC rasterDesc;

	//Setup the raster description which will determine how and what polygons will be drawn.
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_NONE;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	return m_Device->CreateRasterizerState(&rasterDesc, &m_RasterState);
}

ID3D11DeviceContext *Graphics::getDeviceContext()
{
	return m_DeviceContext;
}

ID3D11Device *Graphics::getDevice()
{
	return m_Device;
}