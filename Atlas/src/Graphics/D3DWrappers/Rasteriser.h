#pragma once
#include "Graphics\Bindable.h"

namespace Atlas
{
	//A rasteriser defines certain attributes of how the GPU will handle the processing of shapes
	class Rasteriser : public Bindable
	{
	public:
		//Create the rasteriser
		static std::shared_ptr<Rasteriser> Create(bool antialiedLine = false, bool multiSampling = false, D3D11_CULL_MODE cullMode = D3D11_CULL_BACK, D3D11_FILL_MODE fillMode = D3D11_FILL_SOLID,
			bool frontAnticlockwise = false, int depthBias = 0, float DepthBiasClamp = 0, float SlopeScaledDepthBias = 0, bool enableDepthClip = true, bool enableScissorCulling = false);

		//Generate the UID of the rasteriser
		static std::string GenerateUID(bool antialiedLine, bool multiSampling, D3D11_CULL_MODE cullMode, D3D11_FILL_MODE fillMode,
			bool frontAnticlockwise, int depthBias, float DepthBiasClamp, float SlopeScaledDepthBias, bool enableDepthClip, bool enableScissorCulling);

		//Bind the rasteriser to the immediate context
		virtual void ImmidiateBind() override;
		//Bind the rasteriser to the deferred context
		virtual void Bind(wrl::ComPtr<ID3D11DeviceContext> context) override;
	private:
		//The constructor of the class
		Rasteriser(bool antialiedLine, bool multiSampling, D3D11_CULL_MODE cullMode, D3D11_FILL_MODE fillMode,
			bool frontAnticlockwise, int depthBias, float DepthBiasClamp, float SlopeScaledDepthBias, bool enableDepthClip, bool enableScissorCulling);

		//The raw rasteriser data
		wrl::ComPtr<ID3D11RasterizerState> m_Rasteriser;
	};
}