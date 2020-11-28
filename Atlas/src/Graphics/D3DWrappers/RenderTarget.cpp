#include "pch.h"
#include "RenderTarget.h"
#include "Graphics/Graphics.h"
#include "Graphics/DxgiInfoManager.h"

namespace Atlas
{
	RenderTarget::RenderTarget(uint width, uint height)
		: m_Width(width), m_Height(height)
	{
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
		wrl::ComPtr<ID3D11Texture2D> Texture;
		AT_CHECK_GFX_INFO(Graphics::GetDevice()->CreateTexture2D(
			&textureDesc, nullptr, &Texture
		));

		D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc = {};
		renderTargetViewDesc.Format = textureDesc.Format;
		renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		renderTargetViewDesc.Texture2D = D3D11_TEX2D_RTV{ 0 };
		AT_CHECK_GFX_INFO(Graphics::GetDevice()->CreateRenderTargetView(
			Texture.Get(), &renderTargetViewDesc, &m_RenderTargetView
		));
	}

	RenderTarget::RenderTarget(ID3D11Texture2D* texture)
	{
		D3D11_TEXTURE2D_DESC textureDesc = {};
		texture->GetDesc(&textureDesc);
		m_Width = textureDesc.Width;
		m_Height = textureDesc.Height;

		D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc = {};
		renderTargetViewDesc.Format = textureDesc.Format;
		renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		renderTargetViewDesc.Texture2D = D3D11_TEX2D_RTV{ 0 };
		AT_CHECK_GFX_INFO(Graphics::GetDevice()->CreateRenderTargetView(
			texture, &renderTargetViewDesc, &m_RenderTargetView
		));
	}

	RenderTarget::RenderTarget() { }

	void RenderTarget::Clear()
	{
		Clear(0, 0, 0, 0);
	}

	void RenderTarget::Clear(float r, float g, float b, float a)
	{
		//The back buffer is filled with the color
		float color[4] = { r, g, b, a };
		AT_CHECK_GFX_INFO_VOID(Graphics::GetContext()->ClearRenderTargetView(m_RenderTargetView.Get(), color));
	}

	void RenderTarget::ImmidiateBind()
	{
		ImmidiateBind(nullptr);
	}

	void RenderTarget::ImmidiateBind(ID3D11DepthStencilView* depthStencilBuffer)
	{
		AT_CHECK_GFX_INFO_VOID(Graphics::GetContext()->OMSetRenderTargets(1, m_RenderTargetView.GetAddressOf(), depthStencilBuffer));
	}

	void RenderTarget::Bind(wrl::ComPtr<ID3D11DeviceContext> context)
	{
		Bind(context, nullptr);
	}

	void RenderTarget::Bind(wrl::ComPtr<ID3D11DeviceContext> context, ID3D11DepthStencilView* depthStencilBuffer)
	{
		AT_CHECK_GFX_INFO_VOID(context->OMSetRenderTargets(1, m_RenderTargetView.GetAddressOf(), depthStencilBuffer));
	}

	void RenderTarget::Copy(std::shared_ptr<Buffer> buffer)
	{
		auto data = std::static_pointer_cast<RenderTarget>(buffer);
		m_RenderTargetView = data->GetTargetView();
		m_Width = data->GetWidth();
		m_Height = data->GetHeight();
	}
}