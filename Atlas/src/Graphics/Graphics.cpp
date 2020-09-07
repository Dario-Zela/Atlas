#include "pch.h"
#include "Graphics.h"
#include "Graphics/DX11Exception.h"

#if AT_DEBUG
#define AT_CHECK_GFX_INFO(x) m_InfoManager.Set(); {HRESULT hr = x; if(FAILED(hr)) throw DX11Exception(__LINE__, __FILE__, hr, m_InfoManager.GetMessages());} 
#else
#define AT_CHECK_GFX_INFO(x) AT_CHECK_GFX(x)
#endif
namespace Atlas
{
	Graphics::Graphics()
	{
	}

	void Graphics::Init(HWND hwnd)
	{

#ifdef AT_DEBUG
		m_InfoManager.Init();
#endif  

		DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
		swapChainDesc.BufferDesc.Width = 0;
		swapChainDesc.BufferDesc.Height = 0;
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 0;
		swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.BufferCount = 1;
		swapChainDesc.OutputWindow = hwnd;
		swapChainDesc.Flags = 0;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		swapChainDesc.Windowed = TRUE;

		AT_CHECK_GFX_INFO(D3D11CreateDeviceAndSwapChain(
			nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr,
#ifdef AT_DEBUG
			D3D11_CREATE_DEVICE_DEBUG,
#else
			0,
#endif
			nullptr,
			0,
			D3D11_SDK_VERSION,
			&swapChainDesc,
			&m_SwapChain,
			&m_Device,
			nullptr,
			&m_Context
		))

		wrl::ComPtr<ID3D11Resource> backBuffer = nullptr;
		AT_CHECK_GFX_INFO(m_SwapChain->GetBuffer(0, __uuidof(ID3D11Resource), &backBuffer))
		AT_CHECK_GFX_INFO(m_Device->CreateRenderTargetView(backBuffer.Get(), nullptr, &m_Buffer))
	}

	void Graphics::EndFrame()
	{
		HRESULT result = m_SwapChain->Present(1, 0);
		AT_CHECK_GFX_INFO(result == DXGI_ERROR_DEVICE_REMOVED ? m_Device->GetDeviceRemovedReason() : result);
	}

	void Graphics::ClearScreen(float r, float g, float b, float a)
	{
		float color[4] = { r, g, b, a };
		AT_ATTEMPT(m_Context->ClearRenderTargetView(m_Buffer.Get(), color));
	}
}