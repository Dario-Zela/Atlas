#include "pch.h"
#include "RenderTarget.h"
#include "Graphics/Graphics.h"
#include "Graphics/DxgiInfoManager.h"
#include "Graphics/D3DWrappers/Texture.h"

namespace Atlas
{
	RenderTarget::RenderTarget(uint width, uint height)
		: m_Width(width), m_Height(height)
	{
		AT_CORE_ASSERT(width * height > 0, "The width and height of the render target must be greater then 0")

		//Create the descriptor for the texture 
		//encapsulating the render target
		D3D11_TEXTURE2D_DESC textureDesc = {};
		textureDesc.Width = width;
		textureDesc.Height = height;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = 0;

		//Generate the texture
		wrl::ComPtr<ID3D11Texture2D> texture;
		AT_CHECK_GFX_INFO(Graphics::GetDevice()->CreateTexture2D(
			&textureDesc, nullptr, &texture
		));

		//Create the render target
		D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc = {};
		renderTargetViewDesc.Format = textureDesc.Format;
		renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		renderTargetViewDesc.Texture2D = D3D11_TEX2D_RTV{ 0 };
		AT_CHECK_GFX_INFO(Graphics::GetDevice()->CreateRenderTargetView(
			texture.Get(), &renderTargetViewDesc, &m_RenderTargetView
		));

		//Get the resource of the render target view
		wrl::ComPtr<ID3D11Resource> resource;
		m_RenderTargetView->GetResource(&resource);

		//Create the shader resource view on the render target
		D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceView = {};
		shaderResourceView.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		shaderResourceView.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		shaderResourceView.Texture2D.MostDetailedMip = 0;
		shaderResourceView.Texture2D.MipLevels = 1;
		AT_CHECK_GFX_INFO(Graphics::GetDevice()->CreateShaderResourceView(
			resource.Get(), &shaderResourceView, &m_ShaderResourceView
		));
	}

	RenderTarget::RenderTarget(ID3D11Texture2D* texture)
	{
		//Get the width and height of the texture
		D3D11_TEXTURE2D_DESC textureDesc = {};
		texture->GetDesc(&textureDesc);
		m_Width = textureDesc.Width;
		m_Height = textureDesc.Height;

		//Create the render target
		D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc = {};
		renderTargetViewDesc.Format = textureDesc.Format;
		renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		renderTargetViewDesc.Texture2D = D3D11_TEX2D_RTV{ 0 };
		AT_CHECK_GFX_INFO(Graphics::GetDevice()->CreateRenderTargetView(
			texture, &renderTargetViewDesc, &m_RenderTargetView
		));

		//Get the resource of the render target view
		wrl::ComPtr<ID3D11Resource> resource;
		m_RenderTargetView->GetResource(&resource);

		//Create the shader resource view descriptor on the render target
		D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceView = {};
		shaderResourceView.Format = textureDesc.Format;
		shaderResourceView.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		shaderResourceView.Texture2D.MostDetailedMip = 0;
		shaderResourceView.Texture2D.MipLevels = 1;

		//Attempt to create the shader resource view
		try 
		{
			Graphics::GetDevice()->CreateShaderResourceView(
				resource.Get(), &shaderResourceView, &m_ShaderResourceView
			);
		}
		catch(std::exception&)
		{
			//If it is not possible, leave it as a nullptr
			m_ShaderResourceView = nullptr;
		}
	}

	RenderTarget::RenderTarget() { }

	void RenderTarget::Clear()
	{
		//Clear the render target black
		Clear(0, 0, 0, 0);
	}

	void RenderTarget::Clear(float r, float g, float b, float a)
	{
		//The render target is filled with the colour
		float color[4] = { r, g, b, a };
		AT_CHECK_GFX_INFO_VOID(Graphics::GetContext()->ClearRenderTargetView(m_RenderTargetView.Get(), color));
	}

	void RenderTarget::ImmediateBind()
	{
		//Immediately bind the render target with 
		//no depth stencil view
		ImmediateBind(nullptr);
	}

	void RenderTarget::ImmediateBind(ID3D11DepthStencilView* depthStencilView)
	{
		//Immediately bind the render target with the depth stencil view
		AT_CHECK_GFX_INFO_VOID(Graphics::GetContext()->OMSetRenderTargets(1, m_RenderTargetView.GetAddressOf(), depthStencilView));
	}

	std::shared_ptr<Texture> RenderTarget::GetAsTexture(uint slot, uint targets)
	{
		//Check that a shader resource was generated
		AT_CORE_ASSERT(m_ShaderResourceView, "The shader resource view is empty, you are trying to get a bindable\nfrom a non bindable resource")

		//This should not be available from the BindableLib as it would duplicate data
		//And still needs to be remade each frame as the texture data changes during execution
		return std::shared_ptr<Texture>(new Texture(m_ShaderResourceView.Get(), slot, targets));
	}

	void RenderTarget::Bind(wrl::ComPtr<ID3D11DeviceContext> context)
	{
		//Bind the render target on a deferred context
		//with no depth stencil view
		Bind(context, nullptr);
	}

	void RenderTarget::Bind(wrl::ComPtr<ID3D11DeviceContext> context, ID3D11DepthStencilView* depthStencilView)
	{
		//Bind the render target on a deferred context with the depth stencil view
		AT_CHECK_GFX_INFO_VOID(context->OMSetRenderTargets(1, m_RenderTargetView.GetAddressOf(), depthStencilView));
	}

	void RenderTarget::Equate(std::shared_ptr<Buffer> buffer)
	{
		//Convert the shared pointer from a generic buffer
		//to a render target
		auto data = std::static_pointer_cast<RenderTarget>(buffer);

		//Reassign the pointer of the attributes to those of
		//of the input
		m_RenderTargetView = data->GetTargetView();
		m_ShaderResourceView = data->m_ShaderResourceView;

		//And reassigns the non-pointer attributes
		m_Width = data->GetWidth();
		m_Height = data->GetHeight();
	}
}