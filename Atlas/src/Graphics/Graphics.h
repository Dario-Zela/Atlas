#pragma once
#include "Graphics/D3DWrappers/D3D11CommonHeaders.h"

namespace Atlas
{
	class RenderTarget;
	class ViewPort;

	class Graphics
	{
	public:
		Graphics() = default;
		Graphics(const Graphics&) = delete;
		Graphics& operator=(const Graphics&) = delete;
		~Graphics();

		void Init(HWND hwnd);
		static void EndFrame(uint syncRate = 1);
		
		static void ImmediateDraw(uint vertexCount);
		static void ImmediateDrawIndexed(uint indexCount);

		static void Draw(uint vertexCount, wrl::ComPtr<ID3D11DeviceContext> context);
		static void DrawIndexed(uint indexCount, wrl::ComPtr<ID3D11DeviceContext> context);

		static std::shared_ptr<ViewPort> GetDefaultViewPort();

		static bool IsInitialised();

		static wrl::ComPtr<ID3D11Device> GetDevice();
		static wrl::ComPtr<ID3D11DeviceContext> GetContext();
		static std::shared_ptr<RenderTarget> GetRenderTarget();
	private:
		static Graphics* s_Instance;

		wrl::ComPtr<IDXGISwapChain> m_SwapChain;
		wrl::ComPtr<ID3D11Device> m_Device;
		wrl::ComPtr<ID3D11DeviceContext> m_Context;
		std::shared_ptr<RenderTarget> m_RenderTarget;

		std::shared_ptr<ViewPort> m_FullScreenPort;
	};
}
