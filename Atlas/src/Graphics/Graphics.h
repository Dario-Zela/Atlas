#pragma once
#pragma comment(lib, "d3d11.lib")
#include <d3d11.h>
#include <wrl.h>
namespace wrl = Microsoft::WRL;

#include "Graphics/DxgiInfoManager.h"

namespace Atlas
{
	class VertexBuffer;
	class IndexBuffer;
	class VertexShader;
	class PixelShader;
	class InputLayout;
	class ViewPort;

	class Graphics
	{
	public:
		Graphics() = default;
		Graphics(const Graphics&) = delete;
		Graphics& operator=(const Graphics&) = delete;
		~Graphics() = default;

		void Init(HWND hwnd);
		void EndFrame(uint syncRate = 1);
		void ClearScreen(float r, float g, float b, float a = 1.0f);
		
		void SetPrimitiveTopology(uint topology);
		void SetRenderTarget();

		void Draw(uint vertexCount);
		void DrawIndexed(uint indexCount);

		void CleanDraw(VertexBuffer& vertexBuffer, uint stride, uint offset, uint count, VertexShader& vertexShader, PixelShader& pixelShader, InputLayout& inputLayout, uint topology, ViewPort viewPort);
		void CleanDrawIndexed(VertexBuffer& vertexBuffer, uint stride, uint offset1, IndexBuffer& indexBuffer, uint offset2, VertexShader& vertexShader, PixelShader& pixelShader, InputLayout& inputLayout, uint topology, ViewPort viewPort);

		static wrl::ComPtr<IDXGISwapChain> GetSwapChain() { return s_Instance->m_SwapChain; }
		static wrl::ComPtr<ID3D11Device> GetDevice() { return s_Instance->m_Device; }
		static wrl::ComPtr<ID3D11DeviceContext> GetContext() { return s_Instance->m_Context; }
		static wrl::ComPtr<ID3D11RenderTargetView> GetBackBuffer() { return s_Instance->m_Buffer; }

	private:
		static Graphics* s_Instance;

		wrl::ComPtr<IDXGISwapChain> m_SwapChain;
		wrl::ComPtr<ID3D11Device> m_Device;
		wrl::ComPtr<ID3D11DeviceContext> m_Context;
		wrl::ComPtr<ID3D11RenderTargetView> m_Buffer;
	};
}
