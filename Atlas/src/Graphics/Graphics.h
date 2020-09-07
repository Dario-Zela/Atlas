#pragma once
#pragma comment(lib, "d3d11.lib")
#include <d3d11.h>
#include <wrl.h>
namespace wrl = Microsoft::WRL;

#include "Graphics/DxgiInfoManager.h"

namespace Atlas
{
	class Graphics
	{
	public:
		Graphics();
		void Init(HWND hwnd);
		void EndFrame();
		void ClearScreen(float r, float g, float b, float a = 1.0f);
	private:
#ifdef AT_DEBUG
		DxgiInfoManager m_InfoManager;
#endif
		wrl::ComPtr<IDXGISwapChain> m_SwapChain;
		wrl::ComPtr<ID3D11Device> m_Device;
		wrl::ComPtr<ID3D11DeviceContext> m_Context;
		wrl::ComPtr<ID3D11RenderTargetView> m_Buffer;
	};
}
