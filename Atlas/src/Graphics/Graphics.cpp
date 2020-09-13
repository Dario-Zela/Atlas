#include "pch.h"
#include "Graphics.h"
#include "Graphics/DX11Exception.h"

#include "Graphics/Buffers.h"
#include "Graphics/Shaders.h"
#include "Graphics/InputLayout.h"
#include "Graphics/ViewPort.h"

namespace Atlas
{
	Graphics* Graphics::s_Instance = nullptr;

	void Graphics::Init(HWND hwnd)
	{
		if (!s_Instance)
			s_Instance = this;
		else
			return;

		//If it is in debug mode
		//Initialise the info manager
#ifdef AT_DEBUG
		DxgiInfoManager::Init();
#endif  
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
		AT_CHECK_GFX_INFO(m_SwapChain->GetBuffer(0, __uuidof(ID3D11Resource), &backBuffer))
		AT_CHECK_GFX_INFO(m_Device->CreateRenderTargetView(backBuffer.Get(), nullptr, &m_Buffer))
	}

	void Graphics::EndFrame(uint syncRate)
	{
		//At the end of the frame, the swap chain presents the back buffer
		//1 shows VSync
		HRESULT result = m_SwapChain->Present(syncRate, 0);
		AT_CHECK_GFX_INFO(result == DXGI_ERROR_DEVICE_REMOVED ? m_Device->GetDeviceRemovedReason() : result);
	}

	void Graphics::ClearScreen(float r, float g, float b, float a)
	{
		//The back buffer is filled with the color
		float color[4] = { r, g, b, a };
		AT_CHECK_GFX_INFO_VOID(m_Context->ClearRenderTargetView(m_Buffer.Get(), color));
	}

	void Graphics::SetPrimitiveTopology(uint topology)
	{
		AT_CHECK_GFX_INFO_VOID(m_Context->IASetPrimitiveTopology((D3D11_PRIMITIVE_TOPOLOGY)topology));
	}

	void Graphics::SetRenderTarget()
	{
		AT_CHECK_GFX_INFO_VOID(m_Context->OMSetRenderTargets(1, m_Buffer.GetAddressOf(), nullptr));
	}

	void Graphics::DrawIndexed(uint indexCount)
	{
		AT_CHECK_GFX_INFO_VOID(m_Context->DrawIndexed(indexCount, 0, 0));
	}

	void Graphics::Draw(uint vertexCount)
	{
		AT_CHECK_GFX_INFO_VOID(m_Context->Draw(vertexCount, 0));
	}

	void Graphics::CleanDraw(VertexBuffer& vertexBuffer, uint stride, uint offset, uint count, VertexShader& vertexShader, PixelShader& pixelShader, InputLayout& inputLayout, uint topology, ViewPort viewPort)
	{
		vertexBuffer.Bind(stride, offset);
		vertexShader.Bind();
		pixelShader.Bind();
		inputLayout.Bind();
		SetPrimitiveTopology(topology);
		viewPort.Bind();
		SetRenderTarget();
		Draw(count);
		vertexShader.Unbind();
		pixelShader.Unbind();
		inputLayout.Unbind();
	}

	void Graphics::CleanDrawIndexed(VertexBuffer& vertexBuffer, uint stride, uint offset1, IndexBuffer& indexBuffer, uint offset2, VertexShader& vertexShader, PixelShader& pixelShader, InputLayout& inputLayout, uint topology, ViewPort viewPort)
	{
		vertexBuffer.Bind(stride, offset1);
		indexBuffer.Bind(offset2);
		vertexShader.Bind();
		pixelShader.Bind();
		inputLayout.Bind();
		SetPrimitiveTopology(topology);
		viewPort.Bind();
		SetRenderTarget();
		DrawIndexed(indexBuffer.GetCount());
		vertexShader.Unbind();
		pixelShader.Unbind();
		inputLayout.Unbind();
	}
}
