#pragma once
#pragma comment(lib, "d3d11.lib")
#include <d3d11.h>
#include <wrl.h>
namespace wrl = Microsoft::WRL;

#include "Graphics/D3DWrappers/RenderTarget.h"

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

		static void SetPrimitiveTopology(uint topology);
		
		static void Draw(uint vertexCount);
		static void DrawIndexed(uint indexCount);

		static void BindDefaultViewPort();

		static bool IsInitialised();

		static wrl::ComPtr<ID3D11Device> GetDevice() { return s_Instance->m_Device; }
		static wrl::ComPtr<ID3D11DeviceContext> GetContext() { return s_Instance->m_Context; }
		static std::shared_ptr<RenderTarget> GetRenderTarget() { return s_Instance->m_RenderTarget; }
	private:
		static Graphics* s_Instance;

		wrl::ComPtr<IDXGISwapChain> m_SwapChain;
		wrl::ComPtr<ID3D11Device> m_Device;
		wrl::ComPtr<ID3D11DeviceContext> m_Context;
		std::shared_ptr<RenderTarget> m_RenderTarget;

		D3D11_VIEWPORT m_FullScreenPort;
	};
}
