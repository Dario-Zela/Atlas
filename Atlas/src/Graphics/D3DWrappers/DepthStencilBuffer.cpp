#include "pch.h"
#include "DepthStencilBuffer.h"
#include "Graphics/Graphics.h"
#include "Graphics/DxgiInfoManager.h"
#include "Graphics/D3DWrappers/RenderTarget.h"
#include "Core/Input.h"

namespace Atlas
{
	DepthStencilBuffer::DepthStencilBuffer(uint width, uint height)
	{
		//Create the depth state
		D3D11_DEPTH_STENCIL_DESC desc = {};
		desc.DepthEnable = TRUE;
		desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		desc.DepthFunc = D3D11_COMPARISON_LESS;

		wrl::ComPtr<ID3D11DepthStencilState> depthState;
		AT_CHECK_GFX_INFO(Graphics::GetDevice()->CreateDepthStencilState(&desc, &depthState));

		//Bind the state
		AT_CHECK_GFX_INFO_VOID(Graphics::GetContext()->OMSetDepthStencilState(depthState.Get(), 1));


		//Create the texture
		wrl::ComPtr<ID3D11Texture2D> depthTexture = {};
		D3D11_TEXTURE2D_DESC descTex;
		descTex.Width = (uint)width;
		descTex.Height = (uint)height;
		descTex.MipLevels = 1;
		descTex.ArraySize = 1;
		descTex.Format = DXGI_FORMAT_D32_FLOAT;
		descTex.SampleDesc.Count = 1;
		descTex.SampleDesc.Quality = 0;
		descTex.Usage = D3D11_USAGE_DEFAULT;
		descTex.CPUAccessFlags = 0;
		descTex.MiscFlags = 0;
		descTex.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		AT_CHECK_GFX_INFO(Graphics::GetDevice()->CreateTexture2D(&descTex, nullptr, &depthTexture));

		//Create Texture View
		D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
		descDSV.Format = DXGI_FORMAT_D32_FLOAT;
		descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		descDSV.Texture2D.MipSlice = 0;
		AT_CHECK_GFX_INFO(Graphics::GetDevice()->CreateDepthStencilView(depthTexture.Get(), &descDSV, &m_DepthStencilView));
	}

	std::shared_ptr<DepthStencilBuffer> DepthStencilBuffer::Create()
	{
		auto [width, height] = Input::GetWindowSize();

		return std::move(Create(width, height));
	}

	void DepthStencilBuffer::Clear()
	{
		//The Depth Stencil is cleared
		AT_CHECK_GFX_INFO_VOID(Graphics::GetContext()->ClearDepthStencilView(m_DepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1, 0));
	}

	void DepthStencilBuffer::Bind()
	{
		AT_CHECK_GFX_INFO_VOID(Graphics::GetContext()->OMSetRenderTargets(1, nullptr, m_DepthStencilView.Get()));
	}

	void DepthStencilBuffer::Bind(RenderTarget* renderTarget)
	{
		renderTarget->Bind(m_DepthStencilView.Get());
	}

	void DepthStencilBuffer::Copy(std::shared_ptr<Buffer> buffer)
	{
		auto data = std::static_pointer_cast<DepthStencilBuffer>(buffer);
		m_DepthStencilView = data->GetDepthStencilBuffer();
	}
}
