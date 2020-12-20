#pragma once
#include <d3d11.h>
#include <wrl.h>
namespace wrl = Microsoft::WRL;

#include "Graphics/Buffer.h"
#include "Graphics/D3DWrappers/DepthStencilBuffer.h"

namespace Atlas
{
	class RenderTarget : public Buffer
	{
	public:
		RenderTarget(uint width, uint height);
		RenderTarget(ID3D11Texture2D* texture);
		RenderTarget();

		static std::shared_ptr<RenderTarget> Create(uint width, uint height) { return std::make_shared<RenderTarget>(width, height); }
		static std::shared_ptr<RenderTarget> Create(ID3D11Texture2D* texture) { return std::make_shared<RenderTarget>(texture); }
		static std::shared_ptr<RenderTarget> CreateEmpty() { return std::make_shared<RenderTarget>(); }

		uint GetWidth() { return m_Width; }
		uint GetHeight() { return m_Height; }

		bool IsValid() { return m_RenderTargetView && m_Width != 0 && m_Height != 0; }

		std::string GetTypeName() override { return "RenderTarget"; }

		void Clear();
		void Clear(float r, float g, float b, float a = 1.0f);

		void ImmidiateBind();
		void ImmidiateBind(ID3D11DepthStencilView* depthStencilBuffer);

		std::shared_ptr<Texture> GetAsTexture(uint slot);

		void Bind(wrl::ComPtr<ID3D11DeviceContext> context);
		void Bind(wrl::ComPtr<ID3D11DeviceContext> context, ID3D11DepthStencilView* depthStencilBuffer);

		void Copy(std::shared_ptr<Buffer> buffer) override;

		wrl::ComPtr<ID3D11RenderTargetView> GetTargetView() { return m_RenderTargetView; }
	private:
		wrl::ComPtr<ID3D11RenderTargetView> m_RenderTargetView;
		wrl::ComPtr<ID3D11Texture2D> m_Texture;

		uint m_Width;
		uint m_Height;
	};
}