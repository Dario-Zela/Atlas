#pragma once
#include <d3d11.h>
#include <wrl.h>
namespace wrl = Microsoft::WRL;
#include "Graphics/Buffer.h"

namespace Atlas
{
	//Define classes used later
	class RenderTarget;
	class Texture;

	//A type of buffer that defines the depths and stencil values
	//of a draw call
	class DepthStencilBuffer : public Buffer
	{
	public:
		//Creates an depth buffer of a certain size
		static std::shared_ptr<DepthStencilBuffer> Create(uint width, uint height) { return std::shared_ptr<DepthStencilBuffer>(std::move(new DepthStencilBuffer(width, height))); }
		//Creates a depth buffer the size of the screen
		static std::shared_ptr<DepthStencilBuffer> Create();
		//Creates an empty depth buffer
		//This cannot be bound, but can be used to create a sink so that it
		//can be filled by the previous pass
		static std::shared_ptr<DepthStencilBuffer> CreateEmpty() { return std::shared_ptr<DepthStencilBuffer>(std::move(new DepthStencilBuffer()));}
		
		//Clears the depth buffer
		void Clear();

		//Checks if the depth buffer is valid
		bool IsValid() { return m_DepthStencilView; }

		//Immediately bids the depths stencil view
		void ImmidiateBind();
		//Immediately binds the depths stencil view with a render target
		void ImmidiateBind(RenderTarget* renderTarget);

		//Gets the depths stencil view as a texture
		//Can only be used if it is not bound as a depths stencil view
		std::shared_ptr<Texture> GetAsTexture(uint slot, uint targets);

		//Binds the depths stencil view to a deferred context
		void Bind(wrl::ComPtr<ID3D11DeviceContext> context);
		//Binds the depths stencil view to a deferred context with a render target
		void Bind(wrl::ComPtr<ID3D11DeviceContext> context, RenderTarget* renderTarget);

		//Turns the depths stencil view into the input depths stencil view
		void Equate(std::shared_ptr<Buffer> buffer) override;

		//Gets the width of the depths stencil view
		uint GetWidth() { return m_Width; }
		//Gets the height of the depths stencil view
		uint GetHeight() { return m_Height; }

		//Gets the name of the buffer type
		std::string GetTypeName() override { return "DepthStencilBuffer"; }

		//Gets the inner depth stencil view
		wrl::ComPtr<ID3D11DepthStencilView> GetDepthStencilBuffer() { return m_DepthStencilView; }
	private:
		//The constructors for the appropriate create method
		DepthStencilBuffer(uint width, uint height);
		DepthStencilBuffer();

		//The raw depth stencil view
		wrl::ComPtr<ID3D11DepthStencilView> m_DepthStencilView;
		//The shader resource view indicating the texture 
		//of the depth stencil view
		wrl::ComPtr<ID3D11ShaderResourceView> m_ShaderResourceView;

		//The width of the depth stencil view
		uint m_Width;
		//The height of the depth stencil view
		uint m_Height;
	};
}