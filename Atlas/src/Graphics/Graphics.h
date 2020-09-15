#pragma once
#pragma comment(lib, "d3d11.lib")
#include <d3d11.h>
#include <wrl.h>
namespace wrl = Microsoft::WRL;

#include "Graphics/DxgiInfoManager.h"

namespace Atlas
{
	//Decleres the wrapper classes for d3d objects
	class VertexBuffer;
	class IndexBuffer;
	class VertexShader;
	class PixelShader;
	class InputLayout;
	class ViewPort;

	//A wrapper class for draw call functions.
	class Graphics
	{
	public:
		Graphics() = default;
		Graphics(const Graphics&) = delete;
		Graphics& operator=(const Graphics&) = delete;
		~Graphics() = default;

		//Initialises the graphics object
		void Init(HWND hwnd);
		//Presents the backbuffer to the front buffer
		static void EndFrame(uint syncRate = 1);
		//Clears the screen
		static void ClearScreen(float r, float g, float b, float a = 1.0f);
		
		//Set the primitive topology being displayed
		static void SetPrimitiveTopology(uint topology);
		//Set the render target to the backbuffer
		static void SetRenderTarget();

		//These call a draw call on all bound elements
		static void Draw(uint vertexCount);
		static void DrawIndexed(uint indexCount);

		//These call a draw call on the items submitted
		static void CleanDraw(VertexBuffer& vertexBuffer, uint stride, uint offset, uint count, VertexShader& vertexShader, PixelShader& pixelShader, InputLayout& inputLayout, uint topology, ViewPort viewPort);
		static void CleanDrawIndexed(VertexBuffer& vertexBuffer, uint stride, uint offset1, IndexBuffer& indexBuffer, uint offset2, VertexShader& vertexShader, PixelShader& pixelShader, InputLayout& inputLayout, uint topology, ViewPort viewPort);

		//Getters for graphics items, used by the wrapper classes
		static wrl::ComPtr<IDXGISwapChain> GetSwapChain() { return s_Instance->m_SwapChain; }
		static wrl::ComPtr<ID3D11Device> GetDevice() { return s_Instance->m_Device; }
		static wrl::ComPtr<ID3D11DeviceContext> GetContext() { return s_Instance->m_Context; }
		static wrl::ComPtr<ID3D11RenderTargetView> GetBackBuffer() { return s_Instance->m_Buffer; }

	private:
		//The instance of the graphics element
		static Graphics* s_Instance;

		//The d3d elements
		wrl::ComPtr<IDXGISwapChain> m_SwapChain;
		wrl::ComPtr<ID3D11Device> m_Device;
		wrl::ComPtr<ID3D11DeviceContext> m_Context;
		wrl::ComPtr<ID3D11RenderTargetView> m_Buffer;
	};
}
