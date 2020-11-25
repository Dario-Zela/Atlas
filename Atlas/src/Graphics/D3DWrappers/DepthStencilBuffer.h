#pragma once
#include <d3d11.h>
#include <wrl.h>
namespace wrl = Microsoft::WRL;


namespace Atlas
{
	class RenderTarget;

	class DepthStencilBuffer
	{
	public:
		DepthStencilBuffer(uint width, uint height);
		void Clear();
		void Bind();
		void Bind(RenderTarget* renderTarget);

		wrl::ComPtr<ID3D11DepthStencilView> GetDepthStencilBuffer() { return m_DepthStencilView; }
	private:
		wrl::ComPtr<ID3D11DepthStencilView> m_DepthStencilView;
	};
}