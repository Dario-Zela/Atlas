#pragma once
#pragma comment(lib, "d3d11.lib")
#include <d3d11.h>
#include <wrl.h>
namespace wrl = Microsoft::WRL;

#include "Graphics/D3DWrappers/RenderTarget.h"
#include "Graphics/D3DWrappers/ViewPort.h"

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
		
		static void ImmidiateDraw(uint vertexCount);
		static void ImmidiateDrawIndexed(uint indexCount);

		static void Draw(uint vertexCount, wrl::ComPtr<ID3D11DeviceContext> context);
		static void DrawIndexed(uint indexCount, wrl::ComPtr<ID3D11DeviceContext> context);

		static std::shared_ptr<ViewPort> GetDefaultViewPort() { return s_Instance->m_FullScreenPort; }

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

		std::shared_ptr<ViewPort> m_FullScreenPort;
	};
}
