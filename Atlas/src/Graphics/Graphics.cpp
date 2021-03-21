#include "pch.h"
#include "Graphics.h"
#include "Core/Input.h"
#include "Graphics/BindableLib.h"
#include "Graphics/DxgiInfoManager.h"

#include "Graphics/D3DWrappers/RenderTarget.h"
#include "Graphics/D3DWrappers/ViewPort.h"
#include <d3dcompiler.h>

namespace Atlas
{
	Graphics* Graphics::s_Instance = nullptr;

	Graphics::~Graphics()
	{
		BindableLib::Release();
	}

	void Graphics::Init(HWND hwnd)
	{
		AT_CORE_ASSERT(!s_Instance, "A graphics context has already been constructed")

		s_Instance = this;

		//Define the descriptor for the swap-chain
		DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
		swapChainDesc.BufferDesc.Width = 0;			//Takes the values of the window
		swapChainDesc.BufferDesc.Height = 0;
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;		//A 32 bit RGBA colour format
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;				//No Limit on the refresh rate
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 0;
		swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;		//No scaling specified
		swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;		//No scan line order specified
		swapChainDesc.SampleDesc.Count = 1;				//No anti-aliasing
		swapChainDesc.SampleDesc.Quality = 0;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;		//It declares the buffers as an output
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
		wrl::ComPtr<ID3D11Texture2D> backBuffer = nullptr;
		AT_CHECK_GFX_INFO(m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), &backBuffer));

		m_RenderTarget = RenderTarget::Create(backBuffer.Get());

		//Initialise the Bindables library
		BindableLib::Init();

		//Get the window dimensions
		auto [width, height] = Input::GetWindowSize();

		//Set the default window
		m_FullScreenPort = ViewPort::Create(0, 0, width, height, 0, 1);
	}

	void Graphics::EndFrame(uint syncRate)
	{
		//At the end of the frame, the swap chain presents the back buffer
		//1 shows VSync
		HRESULT result = s_Instance->m_SwapChain->Present(syncRate, 0);
		AT_CHECK_GFX_INFO(result == DXGI_ERROR_DEVICE_REMOVED ? s_Instance->m_Device->GetDeviceRemovedReason() : result);
	}

	void Graphics::ImmediateDraw(uint vertexCount)
	{
		AT_CHECK_GFX_INFO_VOID(s_Instance->m_Context->Draw(vertexCount, 0));
	}

	void Graphics::ImmediateDrawIndexed(uint indexCount)
	{
		AT_CHECK_GFX_INFO_VOID(s_Instance->m_Context->DrawIndexed(indexCount, 0, 0));
	}

	void Graphics::Draw(uint vertexCount, wrl::ComPtr<ID3D11DeviceContext> context)
	{
		AT_CHECK_GFX_INFO_VOID(context->Draw(vertexCount, 0));
	}

	void Graphics::DrawIndexed(uint indexCount, wrl::ComPtr<ID3D11DeviceContext> context)
	{
		AT_CHECK_GFX_INFO_VOID(context->DrawIndexed(indexCount, 0, 0));
	}

	std::shared_ptr<ViewPort> Graphics::GetDefaultViewPort() 
	{ 
		AT_CORE_ASSERT(s_Instance, "The graphics context has been released or \nit is being used before it has been constructed")
		return s_Instance->m_FullScreenPort; 
	}

	bool Graphics::IsInitialised()
	{
		return s_Instance;
	}

	wrl::ComPtr<ID3D11Device> Graphics::GetDevice() 
	{ 
		AT_CORE_ASSERT(s_Instance, "The graphics context has been released or \nit is being used before it has been constructed")
		return s_Instance->m_Device;
	}

	wrl::ComPtr<ID3D11DeviceContext> Graphics::GetContext() 
	{
		AT_CORE_ASSERT(s_Instance, "The graphics context has been released or \nit is being used before it has been constructed") 
		return s_Instance->m_Context; 
	}

	std::shared_ptr<RenderTarget> Graphics::GetRenderTarget() 
	{
		AT_CORE_ASSERT(s_Instance, "The graphics context has been released or \nit is being used before it has been constructed")
		return s_Instance->m_RenderTarget;
	}
}
