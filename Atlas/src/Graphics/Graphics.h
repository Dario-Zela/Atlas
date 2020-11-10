#pragma once
#pragma comment(lib, "d3d11.lib")
#include <d3d11.h>
#include <wrl.h>
namespace wrl = Microsoft::WRL;

namespace Atlas
{
	class Graphics
	{
	public:
		Graphics() = default;
		Graphics(const Graphics&) = delete;
		Graphics& operator=(const Graphics&) = delete;
		~Graphics();

		void Init(HWND hwnd);
		static void EndFrame(uint syncRate = 1);
		static void ClearScreen(float r, float g, float b, float a = 1.0f);

		static void SetPrimitiveTopology(uint topology);
		static void SetRenderTarget();
		
		static void Draw(uint vertexCount);
		static void DrawIndexed(uint indexCount);

		static void BindDefaultViewPort();

		static bool IsInitialised();

		static wrl::ComPtr<ID3D11Device> GetDevice() { return s_Instance->m_Device; }
		static wrl::ComPtr<ID3D11DeviceContext> GetContext() { return s_Instance->m_Context; }

	private:
		static Graphics* s_Instance;

		wrl::ComPtr<IDXGISwapChain> m_SwapChain;
		wrl::ComPtr<ID3D11Device> m_Device;
		wrl::ComPtr<ID3D11DeviceContext> m_Context;
		wrl::ComPtr<ID3D11RenderTargetView> m_Buffer;
		wrl::ComPtr<ID3D11DepthStencilView> m_DepthStencilView;

		D3D11_VIEWPORT m_FullScreenPort;
	};
}
