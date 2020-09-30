#include "pch.h"
#include "Blendable.h"
#include "Graphics/DxgiInfoManager.h"

namespace Atlas
{
	Blendable::Blendable(bool blending, int renderTarget)
	{
		D3D11_BLEND_DESC desc = {};
		auto& tex = desc.RenderTarget[renderTarget];
		if (blending)
		{
			tex.BlendEnable = TRUE;
			tex.SrcBlend = D3D11_BLEND_SRC_ALPHA;
			tex.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
			tex.BlendOp = D3D11_BLEND_OP_ADD;
			tex.SrcBlendAlpha = D3D11_BLEND_ZERO;
			tex.DestBlendAlpha = D3D11_BLEND_ZERO;
			tex.BlendOpAlpha = D3D11_BLEND_OP_ADD;
			tex.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		}
		else
		{
			tex.BlendEnable = FALSE;
			tex.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		}

		AT_CHECK_GFX_INFO(Graphics::GetDevice()->CreateBlendState(&desc, &m_BlendState));
	}

	std::shared_ptr<Blendable> Blendable::Create(bool blending, int renderTarget)
	{
		return std::move(std::make_shared<Blendable>(blending, renderTarget));
	}

	void Blendable::Bind()
	{
		AT_CHECK_GFX_INFO_VOID(Graphics::GetContext()->OMSetBlendState(m_BlendState.Get(), nullptr, 0xFFFFFFFFu));
	}
}