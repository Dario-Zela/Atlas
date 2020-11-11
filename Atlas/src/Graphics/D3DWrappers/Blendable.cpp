#include "pch.h"
#include "Blendable.h"
#include "Graphics/DxgiInfoManager.h"

namespace Atlas
{
	Blendable::Blendable(bool blending, int renderTarget)
	{
		//The descriptor for the blending function
		D3D11_BLEND_DESC blendingDescriptor = {};
		auto& tex = blendingDescriptor.RenderTarget[renderTarget]; //Specifies which render target is blended

		//If the blending is enabled ativate it
		if (blending)
		{
			tex.BlendEnable = TRUE;
			tex.SrcBlend = D3D11_BLEND_SRC_ALPHA;		//It uses source + 1/dest blending
			tex.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
			tex.BlendOp = D3D11_BLEND_OP_ADD;
			tex.SrcBlendAlpha = D3D11_BLEND_ZERO;
			tex.DestBlendAlpha = D3D11_BLEND_ZERO;
			tex.BlendOpAlpha = D3D11_BLEND_OP_ADD;
			tex.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		}
		//Else disable it
		else
		{
			tex.BlendEnable = FALSE;
			tex.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		}

		//Then create the blend state
		AT_CHECK_GFX_INFO(Graphics::GetDevice()->CreateBlendState(&blendingDescriptor, &m_BlendState));
	}

	std::shared_ptr<Blendable> Blendable::Create(bool blending, int renderTarget)
	{
		//This cretes a new blending function and returns it as a shared ptr
		return std::move(std::make_shared<Blendable>(blending, renderTarget));
	}

	void Blendable::Bind()
	{
		//Binds the blend function to the pipeline
		AT_CHECK_GFX_INFO_VOID(Graphics::GetContext()->OMSetBlendState(m_BlendState.Get(), nullptr, 0xFFFFFFFFu));
	}
}