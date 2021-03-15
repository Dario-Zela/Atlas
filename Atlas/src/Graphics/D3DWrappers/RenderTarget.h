#pragma once
#include "Graphics/D3DWrappers/D3D11CommonHeaders.h"
#include "Graphics/Buffer.h"
#include "Graphics/D3DWrappers/DepthStencilBuffer.h"

namespace Atlas
{
	//Wraps a d3d11 render target view
	class RenderTarget : public Buffer
	{
	public:
		//Creates an empty render target of a specific size
		static std::shared_ptr<RenderTarget> Create(uint width, uint height) { return std::shared_ptr<RenderTarget>(std::move(new RenderTarget(width, height))); }
		//Creates a render target from a d3d11 texture
		static std::shared_ptr<RenderTarget> Create(ID3D11Texture2D* texture) { return std::shared_ptr<RenderTarget>(std::move(new RenderTarget(texture))); }
		//Creates an empty render target of 0 width and 0 height
		//This cannot be bound, but can be used to create a sink so that it
		//can be filled by the previous pass
		static std::shared_ptr<RenderTarget> CreateEmpty() { return std::shared_ptr<RenderTarget>(std::move(new RenderTarget())); }

		//Gets the width of the render target
		uint GetWidth() { return m_Width; }
		//Gets the height of the render target
		uint GetHeight() { return m_Height; }

		//Checks of the render target is valid for drawing
		bool IsValid() { return m_RenderTargetView && m_Width != 0 && m_Height != 0; }

		//Gets the type of the class when it is stored as a buffer
		std::string GetTypeName() override { return "RenderTarget"; }

		//Clears the render target black
		void Clear();
		//Clears the render target in a custom colour
		void Clear(float r, float g, float b, float a = 1.0f);

		//Immediately bids the render target
		void ImmediateBind();
		//Immediately binds the render target with a depths stencil view
		void ImmediateBind(ID3D11DepthStencilView* depthStencilView);

		//Gets the render target as a texture
		//Can only be used if it is not bound as a render target
		std::shared_ptr<Texture> GetAsTexture(uint slot, uint targets);

		//Binds the render target to a deferred context
		void Bind(wrl::ComPtr<ID3D11DeviceContext> context);
		//Binds the render target to a deferred context with a depths stencil view
		void Bind(wrl::ComPtr<ID3D11DeviceContext> context, ID3D11DepthStencilView* depthStencilView);

		//Turns the render target into the input render target
		void Equate(std::shared_ptr<Buffer> buffer) override;

		//Gets the raw d3d11 element
		wrl::ComPtr<ID3D11RenderTargetView> GetTargetView() { return m_RenderTargetView; }
	private:
		//Constructors for the appropriate create method
		RenderTarget(uint width, uint height);
		RenderTarget(ID3D11Texture2D* texture);
		RenderTarget();

		//The raw render target view
		wrl::ComPtr<ID3D11RenderTargetView> m_RenderTargetView;
		//The shader resource view indicating the texture 
		//of the render target view if it is possible
		wrl::ComPtr<ID3D11ShaderResourceView> m_ShaderResourceView;

		//The width of the render target view
		uint m_Width;
		//The height of the render target view
		uint m_Height;
	};
}