#include "pch.h"
#include "Blendable.h"
#include "Graphics/DxgiInfoManager.h"
#include "Graphics\BindableLib.h"

namespace Atlas
{
	Blendable::Blendable(bool blending, int renderTarget, D3D11_BLEND sourceBlend, 
		D3D11_BLEND destinationBlend, D3D11_BLEND_OP blendOp, D3D11_BLEND sourceBlendAlpha, 
		D3D11_BLEND destinationBlendAlpha, D3D11_BLEND_OP blendOpAlpha, D3D11_COLOR_WRITE_ENABLE writeMask,
		float blendFactor[4], uint mask)
		: m_Mask(mask)
	{
		if (blendFactor != nullptr)
		{
			m_BlendFactor[0] = blendFactor[0];
			m_BlendFactor[1] = blendFactor[1];
			m_BlendFactor[2] = blendFactor[2];
			m_BlendFactor[3] = blendFactor[3];
		}

		//The descriptor for the blending function
		D3D11_BLEND_DESC blendingDescriptor = {};
		auto& tex = blendingDescriptor.RenderTarget[renderTarget]; //Specifies which render target is blended

		//If the blending is enabled ativate it
		if (blending)
		{
			tex.BlendEnable = TRUE;
			tex.SrcBlend = sourceBlend;
			tex.DestBlend = destinationBlend;
			tex.BlendOp = blendOp;
			tex.SrcBlendAlpha = sourceBlendAlpha;
			tex.DestBlendAlpha = destinationBlendAlpha;
			tex.BlendOpAlpha = blendOpAlpha;
			tex.RenderTargetWriteMask = writeMask;
		}
		//Else disable it
		else
		{
			tex.BlendEnable = FALSE;
			tex.RenderTargetWriteMask = writeMask;
		}

		//Then create the blend state
		AT_CHECK_GFX_INFO(Graphics::GetDevice()->CreateBlendState(&blendingDescriptor, &m_BlendState));
	}

	std::shared_ptr<Blendable> Blendable::Create(bool blending, int renderTarget, D3D11_BLEND sourceBlend,
		D3D11_BLEND destinationBlend, D3D11_BLEND_OP blendOp, D3D11_BLEND sourceBlendAlpha,
		D3D11_BLEND destinationBlendAlpha, D3D11_BLEND_OP blendOpAlpha, D3D11_COLOR_WRITE_ENABLE writeMask,
		float blendFactor[4], uint mask)
	{
		//Get the UID and get the pointer to the data
		std::string UID = GenerateUID(blending, renderTarget, sourceBlend,
			destinationBlend, blendOp, sourceBlendAlpha, destinationBlendAlpha, blendOpAlpha, writeMask, blendFactor, mask);
		auto test = BindableLib::Resolve(UID);

		//If it isn't nullptr, cast it and return it
		if (test)
		{
			return std::static_pointer_cast<Blendable>(test);
		}
		//else return nullptr
		else
		{
			auto newBlend = new Blendable(blending, renderTarget, sourceBlend,
				destinationBlend, blendOp, sourceBlendAlpha, destinationBlendAlpha, blendOpAlpha, writeMask, blendFactor, mask);
			BindableLib::Add(UID, std::shared_ptr<Blendable>(std::move(newBlend)));
			return std::static_pointer_cast<Blendable>(BindableLib::Resolve(UID));
		}
	}

	std::string Blendable::GenerateUID(bool blending, int renderTarget, D3D11_BLEND sourceBlend, D3D11_BLEND destinationBlend, D3D11_BLEND_OP blendOp, D3D11_BLEND sourceBlendAlpha, D3D11_BLEND destinationBlendAlpha, D3D11_BLEND_OP blendOpAlpha, D3D11_COLOR_WRITE_ENABLE writeMask, float blendFactor[4], uint mask)
	{
		return std::string(typeid(Blendable).name()) + '_' + std::to_string(blending) + '_' + std::to_string(renderTarget)
			+ '_' + std::to_string(sourceBlend) + '_' + std::to_string(destinationBlend)
			+ '_' + std::to_string(blendOp) + '_' + std::to_string(sourceBlendAlpha)
			+ '_' + std::to_string(destinationBlendAlpha) + '_' + std::to_string(blendOpAlpha)
			+ (blendFactor == nullptr ? "_0_0_0_0" : '_' + std::to_string(blendFactor[0]) + '_' + std::to_string(blendFactor[1])
			+ '_' + std::to_string(blendFactor[2]) + '_' + std::to_string(blendFactor[3]))
			+ '_' + std::to_string(mask);
	}

	std::shared_ptr<Blendable> Blendable::Get(std::string tag)
	{
		auto test = BindableLib::Resolve(tag);

		//If it isn't nullptr, cast it and return it
		if (test)
		{
			return std::static_pointer_cast<Blendable>(test);
		}
		//else return nullptr
		else
		{
			//Log the lack of bindable
			AT_WARN("There is no blendable that uses the tag {0}", tag)
			return nullptr;
		}
	}

	void Blendable::ImmidiateBind()
	{
		//Binds the blend function to the pipeline
		AT_CHECK_GFX_INFO_VOID(Graphics::GetContext()->OMSetBlendState(m_BlendState.Get(), m_BlendFactor, m_Mask));
	}

	void Blendable::Bind(wrl::ComPtr<ID3D11DeviceContext> context)
	{
		//Binds the blend function to the pipeline
		AT_CHECK_GFX_INFO_VOID(context->OMSetBlendState(m_BlendState.Get(), m_BlendFactor, m_Mask));
	}
}