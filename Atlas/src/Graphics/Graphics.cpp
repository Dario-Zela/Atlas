#include "pch.h"
#include "Graphics.h"
#include "Graphics/DX11Exception.h"
#include <d3dcompiler.h>

#pragma comment(lib, "D3DCompiler.lib")


namespace Atlas
{
	Graphics::Graphics()
	{
	}

	void Graphics::Init(HWND hwnd)
	{
		//If it is in debug mode
		//Initialise the info manager
#ifdef AT_DEBUG
		m_InfoManager.Init();
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
	
	void Graphics::DrawTriangle()
	{
		struct Vertex
		{
			struct {
				float x;
				float y;
			} pos;

			struct {
				byte r;
				byte g;
				byte b;
				byte a;
			} color;
		};

		Vertex data[] =
		{
			{{0, 0.5f}, {0, 0, 255, 255}},
			{{0.5f, -0.5f}, {0, 255, 0, 255}},
			{{-0.5f, -0.5f}, {255, 0, 0, 255}},
			
			{{0, 0}, {255, 0, 255, 255}},
		};

		wrl::ComPtr<ID3D11Buffer> vertexBuffer;
	
		D3D11_BUFFER_DESC bufferDesc = {};
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.CPUAccessFlags = 0;
		bufferDesc.MiscFlags = 0;
		bufferDesc.ByteWidth = sizeof(data);
		bufferDesc.StructureByteStride = sizeof(Vertex);

		D3D11_SUBRESOURCE_DATA vertexData;
		vertexData.pSysMem = data;

		AT_CHECK_GFX_INFO(m_Device->CreateBuffer(&bufferDesc, &vertexData, &vertexBuffer));

		uint strides = sizeof(Vertex);
		uint offsets = 0;

		AT_CHECK_GFX_INFO_VOID(m_Context->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(),&strides, &offsets));

		const unsigned short indices[] =
		{
			3, 0, 1,
			3, 1, 2,
			3, 2, 0
		};

		wrl::ComPtr<ID3D11Buffer> indexBuffer;

		D3D11_BUFFER_DESC bufferDesc2 = {};
		bufferDesc2.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bufferDesc2.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc2.CPUAccessFlags = 0;
		bufferDesc2.MiscFlags = 0;
		bufferDesc2.ByteWidth = sizeof(indices);
		bufferDesc2.StructureByteStride = sizeof(unsigned short);

		D3D11_SUBRESOURCE_DATA indexData;
		indexData.pSysMem = indices;

		AT_CHECK_GFX_INFO(m_Device->CreateBuffer(&bufferDesc2, &indexData, &indexBuffer));

		uint offset = 0;

		AT_CHECK_GFX_INFO_VOID(m_Context->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R16_UINT ,offset));

		wrl::ComPtr<ID3D11VertexShader> vertexShader;
		wrl::ComPtr<ID3DBlob> blob;
		AT_CHECK_GFX_INFO(D3DReadFileToBlob(L"TestVertex.cso", &blob));
		AT_CHECK_GFX_INFO(m_Device->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &vertexShader));
		AT_CHECK_GFX_INFO_VOID(m_Context->VSSetShader(vertexShader.Get(), nullptr, 0));

		wrl::ComPtr<ID3D11InputLayout> inputLayout;
		const D3D11_INPUT_ELEMENT_DESC elementDesc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{ "COLOR", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};
		AT_CHECK_GFX_INFO(m_Device->CreateInputLayout(elementDesc, (uint)std::size(elementDesc), blob->GetBufferPointer(), blob->GetBufferSize(), &inputLayout));
		AT_CHECK_GFX_INFO_VOID(m_Context->IASetInputLayout(inputLayout.Get()));

		wrl::ComPtr<ID3D11PixelShader> pixelShader;
		AT_CHECK_GFX_INFO(D3DReadFileToBlob(L"TestPixel.cso", &blob));
		AT_CHECK_GFX_INFO(m_Device->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &pixelShader));
		AT_CHECK_GFX_INFO_VOID(m_Context->PSSetShader(pixelShader.Get(), nullptr, 0));

		AT_CHECK_GFX_INFO_VOID(m_Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

		AT_CHECK_GFX_INFO_VOID(m_Context->OMSetRenderTargets(1, m_Buffer.GetAddressOf(), nullptr));
		D3D11_VIEWPORT viewPort = {};
		viewPort.TopLeftX = 100;
		viewPort.TopLeftY = 100;
		viewPort.MinDepth = 0;
		viewPort.MaxDepth = 1;
		viewPort.Width = 1024;
		viewPort.Height = 700;
		AT_CHECK_GFX_INFO_VOID(m_Context->RSSetViewports(1, &viewPort));

		AT_CHECK_GFX_INFO_VOID(m_Context->DrawIndexed((uint)std::size(indices), 0, 0));
	}
}
