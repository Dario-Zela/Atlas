#include "pch.h"
#include "Graphics.h"
#include "Graphics/DX11Exception.h"
#include "Core/Input.h"
#include "Graphics/BindableLib.h"

#include "Graphics/DxgiInfoManager.h"

namespace Atlas
{
	Graphics* Graphics::s_Instance = nullptr;

	Graphics::~Graphics()
	{
		BindableLib::Relese();
	}

	void Graphics::Init(HWND hwnd)
	{
		if (!s_Instance)
			s_Instance = this;
		else
			return;

		//Define the descriptor for the swapchain
		DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
		swapChainDesc.BufferDesc.Width = 0;			//Takes the values of the window
		swapChainDesc.BufferDesc.Height = 0;
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;		//A 32 bit RGBA color format
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;				//No Limit on the refresh rate
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 0;
		swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;		//No scaling specified
		swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;		//No scan line order specified
		swapChainDesc.SampleDesc.Count = 1;				//No anti-aliasing
		swapChainDesc.SampleDesc.Quality = 0;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;		//It decleres the buffers as an output
		swapChainDesc.BufferCount = 1;			//1 back buffer and the front buffer
		swapChainDesc.OutputWindow = hwnd;			//Window handle
		swapChainDesc.Flags = 0;				//No flags
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		swapChainDesc.Windowed = TRUE;			//The window has a border

#ifdef AT_DEBUG				//This checks if it is in debug mode and allow for
		int flags = D3D11_CREATE_DEVICE_DEBUG;			//Debug info
#else
		int flags = 0;
#endif

		//The function creates the device, swap chain and device context
		AT_CHECK_GFX_INFO(D3D11CreateDeviceAndSwapChain(
			nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr,
			flags,
			nullptr,
			0,
			D3D11_SDK_VERSION,
			&swapChainDesc,
			&m_SwapChain,
			&m_Device,
			nullptr,
			&m_Context
		))

		//This gets the back buffer render target 
		wrl::ComPtr<ID3D11Resource> backBuffer = nullptr;
		AT_CHECK_GFX_INFO(m_SwapChain->GetBuffer(0, __uuidof(ID3D11Resource), &backBuffer));
		AT_CHECK_GFX_INFO(m_Device->CreateRenderTargetView(backBuffer.Get(), nullptr, &m_Buffer));

		//Set up the z-Buffer

		//Create the depth state
		D3D11_DEPTH_STENCIL_DESC desc = {};
		desc.DepthEnable = TRUE;
		desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		desc.DepthFunc = D3D11_COMPARISON_LESS;

		wrl::ComPtr<ID3D11DepthStencilState> depthState;
		AT_CHECK_GFX_INFO(m_Device->CreateDepthStencilState(&desc, &depthState));
		
		//Bind the state
		AT_CHECK_GFX_INFO_VOID(m_Context->OMSetDepthStencilState(depthState.Get(), 1));

		//Create the texture
		wrl::ComPtr<ID3D11Texture2D> depthTexture = {};
		D3D11_TEXTURE2D_DESC descTex;
		auto [width, height] = Input::GetWindowSize();
		descTex.Width = (uint)width;
		descTex.Height = (uint)height;
		descTex.MipLevels = 1;
		descTex.ArraySize = 1;
		descTex.Format = DXGI_FORMAT_D32_FLOAT;
		descTex.SampleDesc.Count = 1;
		descTex.SampleDesc.Quality = 0;
		descTex.Usage = D3D11_USAGE_DEFAULT;
		descTex.CPUAccessFlags = 0;
		descTex.MiscFlags = 0;
		descTex.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		AT_CHECK_GFX_INFO(m_Device->CreateTexture2D(&descTex, nullptr, &depthTexture));

		//Create Texture View
		D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
		descDSV.Format = DXGI_FORMAT_D32_FLOAT;
		descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		descDSV.Texture2D.MipSlice = 0;
		AT_CHECK_GFX_INFO(m_Device->CreateDepthStencilView(depthTexture.Get(), &descDSV, &m_DepthStencilView));

		//Set the default window
		m_FullScreenPort.TopLeftX = 0;
		m_FullScreenPort.TopLeftY = 0;
		m_FullScreenPort.Width = (float)width;
		m_FullScreenPort.Height = (float)height;
		m_FullScreenPort.MinDepth = 0;
		m_FullScreenPort.MaxDepth = 1;

		//Initialise the Bindable library
		BindableLib::Init();
	}

	void Graphics::EndFrame(uint syncRate)
	{
		//At the end of the frame, the swap chain presents the back buffer
		//1 shows VSync
		HRESULT result = s_Instance->m_SwapChain->Present(syncRate, 0);
		AT_CHECK_GFX_INFO(result == DXGI_ERROR_DEVICE_REMOVED ? s_Instance->m_Device->GetDeviceRemovedReason() : result);
	}

	void Graphics::ClearScreen(float r, float g, float b, float a)
	{
		//The back buffer is filled with the color
		float color[4] = { r, g, b, a };
		AT_CHECK_GFX_INFO_VOID(s_Instance->m_Context->ClearRenderTargetView(s_Instance->m_Buffer.Get(), color));

		//The Depth Stencil is cleared
		AT_CHECK_GFX_INFO_VOID(s_Instance->m_Context->ClearDepthStencilView(s_Instance->m_DepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1, 0));
	}

	void Graphics::SetPrimitiveTopology(uint topology)
	{
		AT_CHECK_GFX_INFO_VOID(s_Instance->m_Context->IASetPrimitiveTopology((D3D11_PRIMITIVE_TOPOLOGY)topology));
	}

	void Graphics::SetRenderTarget()
	{
		AT_CHECK_GFX_INFO_VOID(s_Instance->m_Context->OMSetRenderTargets(1, s_Instance->m_Buffer.GetAddressOf(), s_Instance->m_DepthStencilView.Get()));
	}

	void Graphics::DrawIndexed(uint indexCount)
	{
		AT_CHECK_GFX_INFO_VOID(s_Instance->m_Context->DrawIndexed(indexCount, 0, 0));
	}

	void Graphics::BindDefaultViewPort()
	{
		AT_CHECK_GFX_INFO_VOID(s_Instance->m_Context->RSSetViewports(1, &s_Instance->m_FullScreenPort));
	}

	bool Graphics::IsInitialised()
	{
		return s_Instance;
	}

	void Graphics::Draw(uint vertexCount)
	{
		AT_CHECK_GFX_INFO_VOID(s_Instance->m_Context->Draw(vertexCount, 0));
	}
}
