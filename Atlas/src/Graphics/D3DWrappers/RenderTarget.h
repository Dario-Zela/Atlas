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

		static std::shared_ptr<RenderTarget> Create(uint width, uint height) { return std::make_shared<RenderTarget>(width, height); }
		static std::shared_ptr<RenderTarget> Create(ID3D11Texture2D* texture) { return std::make_shared<RenderTarget>(texture); }

		uint GetWidth() { return m_Width; }
		uint GetHeight() { return m_Height; }

		std::string GetTypeName() override { return "RenderTarget"; }

		void Clear();
		void Clear(float r, float g, float b, float a = 1.0f);

		void Bind();
		void Bind(ID3D11DepthStencilView* depthStencilBuffer);

		void Copy(std::shared_ptr<Buffer> buffer) override;

		bool Debug = false;

		wrl::ComPtr<ID3D11RenderTargetView> GetTargetView() { return m_RenderTargetView; }
	private:
		wrl::ComPtr<ID3D11RenderTargetView> m_RenderTargetView;
		uint m_Width;
		uint m_Height;
	};
}