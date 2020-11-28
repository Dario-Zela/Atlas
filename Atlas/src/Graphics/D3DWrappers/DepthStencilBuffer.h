#pragma once
#include <d3d11.h>
#include <wrl.h>
namespace wrl = Microsoft::WRL;
#include "Graphics/Buffer.h"

namespace Atlas
{
	class RenderTarget;

	class DepthStencilBuffer : public Buffer
	{
	public:
		DepthStencilBuffer(uint width, uint height);
		DepthStencilBuffer();

		static std::shared_ptr<DepthStencilBuffer> Create(uint width, uint height) { return std::make_shared<DepthStencilBuffer>(width, height); }
		static std::shared_ptr<DepthStencilBuffer> Create();

		static std::shared_ptr<DepthStencilBuffer> CreateEmpty() { return std::make_shared<DepthStencilBuffer>(); }

		void Clear();

		bool IsValid() { return m_DepthStencilView; }

		void ImmidiateBind();
		void ImmidiateBind(RenderTarget* renderTarget);

		void Bind(wrl::ComPtr<ID3D11DeviceContext> context);
		void Bind(wrl::ComPtr<ID3D11DeviceContext> context, RenderTarget* renderTarget);

		void Copy(std::shared_ptr<Buffer> buffer) override;

		std::string GetTypeName() override { return "DepthStencilBuffer"; }

		wrl::ComPtr<ID3D11DepthStencilView> GetDepthStencilBuffer() { return m_DepthStencilView; }
	private:
		wrl::ComPtr<ID3D11DepthStencilView> m_DepthStencilView;
	};
}