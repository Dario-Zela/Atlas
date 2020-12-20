#pragma once
#include "Graphics\Bindable.h"

namespace Atlas
{
	class Rasteriser : public Bindable
	{
	public:
		Rasteriser(bool antialiedLine, bool multiSampling, D3D11_CULL_MODE cullMode, D3D11_FILL_MODE fillMode,
			bool frontAnticlockwise, int depthBias, float DepthBiasClamp, float SlopeScaledDepthBias, bool enableDepthClip, bool enableScissorCulling);

		static std::shared_ptr<Rasteriser> Create(bool antialiedLine = false, bool multiSampling = false, D3D11_CULL_MODE cullMode = D3D11_CULL_BACK, D3D11_FILL_MODE fillMode = D3D11_FILL_SOLID,
			bool frontAnticlockwise = false, int depthBias = 0, float DepthBiasClamp = 0, float SlopeScaledDepthBias = 0, bool enableDepthClip = true, bool enableScissorCulling = false);

		static std::string GenerateUID(bool antialiedLine, bool multiSampling, D3D11_CULL_MODE cullMode, D3D11_FILL_MODE fillMode,
			bool frontAnticlockwise, int depthBias, float DepthBiasClamp, float SlopeScaledDepthBias, bool enableDepthClip, bool enableScissorCulling);

		virtual void ImmidiateBind() override;
		virtual void Bind(wrl::ComPtr<ID3D11DeviceContext> context) override;
	private:
		wrl::ComPtr<ID3D11RasterizerState> m_Rasteriser;
	};
}