#include "pch.h"
#include "DepthStencilBuffer.h"
#include "Graphics/Graphics.h"
#include "Graphics/DxgiInfoManager.h"
#include "Graphics/D3DWrappers/RenderTarget.h"
#include "Graphics\D3DWrappers/Texture.h"
#include "Core/Input.h"

namespace Atlas
{
	DepthStencilBuffer::DepthStencilBuffer(uint width, uint height)
		:m_Width(width), m_Height(height)
	{
		//Create the descriptor for the texture 
		//encapsulating the depth stencil view
		D3D11_TEXTURE2D_DESC descTex;
		descTex.Width = (uint)width;
		descTex.Height = (uint)height;
		descTex.MipLevels = 1;
		descTex.ArraySize = 1;
		descTex.Format = DXGI_FORMAT_R24G8_TYPELESS;
		descTex.SampleDesc.Count = 1;
		descTex.SampleDesc.Quality = 0;
		descTex.Usage = D3D11_USAGE_DEFAULT;
		descTex.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
		descTex.CPUAccessFlags = 0;
		descTex.MiscFlags = 0;
		
		//Generate the texture
		wrl::ComPtr<ID3D11Texture2D> texture;
		AT_CHECK_GFX_INFO(Graphics::GetDevice()->CreateTexture2D(&descTex, nullptr, &texture));

		//Create depth stencil view
		D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
		descDSV.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		descDSV.Texture2D.MipSlice = 0;
		AT_CHECK_GFX_INFO(Graphics::GetDevice()->CreateDepthStencilView(texture.Get(), &descDSV, &m_DepthStencilView));

		//Get the resource of the depth stencil view
		wrl::ComPtr<ID3D11Resource> resource;
		m_DepthStencilView->GetResource(&resource);

		//Create the shader resource view on the depth stencil view
		D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceView = {};
		shaderResourceView.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
		shaderResourceView.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		shaderResourceView.Texture2D.MostDetailedMip = 0;
		shaderResourceView.Texture2D.MipLevels = 1;
		AT_CHECK_GFX_INFO(Graphics::GetDevice()->CreateShaderResourceView(
			resource.Get(), &shaderResourceView, &m_ShaderResourceView
		));
	}

	DepthStencilBuffer::DepthStencilBuffer() { }

	std::shared_ptr<DepthStencilBuffer> DepthStencilBuffer::Create()
	{
		//Get the size of the window
		auto [width, height] = Input::GetWindowSize();

		//Create the depth stencil view using that size
		return Create(width, height);
	}

	void DepthStencilBuffer::Clear()
	{
		//The depth stencil view is cleared
		AT_CHECK_GFX_INFO_VOID(Graphics::GetContext()->ClearDepthStencilView(m_DepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1, 0));
	}

	void DepthStencilBuffer::ImmidiateBind()
	{
		//Immediately bind the depth stencil view with no render target
		AT_CHECK_GFX_INFO_VOID(Graphics::GetContext()->OMSetRenderTargets(1, nullptr, m_DepthStencilView.Get()));
	}

	void DepthStencilBuffer::ImmidiateBind(RenderTarget* renderTarget)
	{
		//Immediately bind the depth stencil view using the render target
		renderTarget->ImmidiateBind(m_DepthStencilView.Get());
	}

	std::shared_ptr<Texture> DepthStencilBuffer::GetAsTexture(uint slot, uint targets)
	{
		//This should not be available from the BindableLib as it would duplicate data
		//And still needs to be remade each frame as the texture data changes during execution
		return std::shared_ptr<Texture>(new Texture(m_ShaderResourceView.Get(), slot, targets));
	}

	void DepthStencilBuffer::Bind(wrl::ComPtr<ID3D11DeviceContext> context)
	{
		//Bind the depth stencil view on a deferred context with no render target
		AT_CHECK_GFX_INFO_VOID(context->OMSetRenderTargets(1, nullptr, m_DepthStencilView.Get()));
	}

	void DepthStencilBuffer::Bind(wrl::ComPtr<ID3D11DeviceContext> context, RenderTarget* renderTarget)
	{
		//Bind the depth stencil view on a deferred context using the render target
		renderTarget->Bind(context, m_DepthStencilView.Get());
	}

	void DepthStencilBuffer::Equate(std::shared_ptr<Buffer> buffer)
	{
		//Convert the shared pointer from a generic buffer
		//to a depth stencil view
		auto data = std::static_pointer_cast<DepthStencilBuffer>(buffer);

		//Reassign the pointer of the attributes to those of
		//of the input
		m_DepthStencilView = data->GetDepthStencilBuffer();
		m_ShaderResourceView = data->m_ShaderResourceView;
		
		//And reassigns the non-pointer attributes
		m_Width = data->m_Width;
		m_Height = data->m_Height;
	}
}
