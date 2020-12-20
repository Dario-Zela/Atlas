#pragma once
#include "Graphics/Bindable.h"
#include "Graphics/Graphics.h"

namespace Atlas
{
	//A wrapper over the blending functions in d3d11
	class Blendable : public Bindable
	{
	public:
		//Constructor takes whether or not there should be blending
		//And the render target
		Blendable(bool blending, int renderTarget, D3D11_BLEND sourceBlend,
			D3D11_BLEND destinationBlend, D3D11_BLEND_OP blendOp, D3D11_BLEND sourceBlendAlpha,
			D3D11_BLEND destinationBlendAlpha, D3D11_BLEND_OP blendOpAlpha, D3D11_COLOR_WRITE_ENABLE writeMask,
			float blendFactor[4], uint mask);

		static std::shared_ptr<Blendable> Create(bool blending, int renderTarget = 0, D3D11_BLEND sourceBlend = D3D11_BLEND_SRC_ALPHA,
			D3D11_BLEND destinationBlend = D3D11_BLEND_INV_SRC_ALPHA, D3D11_BLEND_OP blendOp = D3D11_BLEND_OP_ADD, D3D11_BLEND sourceBlendAlpha = D3D11_BLEND_ZERO,
			D3D11_BLEND destinationBlendAlpha = D3D11_BLEND_ZERO, D3D11_BLEND_OP blendOpAlpha = D3D11_BLEND_OP_ADD, D3D11_COLOR_WRITE_ENABLE writeMask = D3D11_COLOR_WRITE_ENABLE_ALL,
			float blendFactor[4] = nullptr, uint mask = 0xFFFFFFFFu);	//A static constructor that creates a shared ptr

		static std::string GenerateUID(bool blending, int renderTarget, D3D11_BLEND sourceBlend,
			D3D11_BLEND destinationBlend, D3D11_BLEND_OP blendOp, D3D11_BLEND sourceBlendAlpha,
			D3D11_BLEND destinationBlendAlpha, D3D11_BLEND_OP blendOpAlpha, D3D11_COLOR_WRITE_ENABLE writeMask,
			float blendFactor[4], uint mask);

		static std::shared_ptr<Blendable> Get(std::string tag);

		void ImmidiateBind() override;	//Binds the blend function to the pipeline
		void Bind(wrl::ComPtr<ID3D11DeviceContext> context) override;
	private:
		wrl::ComPtr<ID3D11BlendState> m_BlendState;
		uint m_Mask;
		float m_BlendFactor[4];
	};
}