#include "pch.h"
#include "Rasteriser.h"
#include "Graphics/Graphics.h"
#include "Graphics/DxgiInfoManager.h"
#include "Graphics\BindableLib.h"

namespace Atlas
{
	Rasteriser::Rasteriser(bool antialiedLine, bool multiSampling, D3D11_CULL_MODE cullMode, D3D11_FILL_MODE fillMode,
		bool frontAnticlockwise, int depthBias, float DepthBiasClamp, float SlopeScaledDepthBias, bool enableDepthClip, bool enableScissorCulling)
	{
		//Create the descriptor
		D3D11_RASTERIZER_DESC desc;
		desc.FillMode = fillMode;
		desc.CullMode = cullMode;
		desc.FrontCounterClockwise = frontAnticlockwise;
		desc.DepthBias = depthBias;
		desc.DepthBiasClamp = DepthBiasClamp;
		desc.SlopeScaledDepthBias = SlopeScaledDepthBias;
		desc.DepthClipEnable = enableDepthClip;
		desc.ScissorEnable = enableScissorCulling;
		desc.MultisampleEnable = multiSampling;
		desc.AntialiasedLineEnable = antialiedLine;

		//And generate the D3D11 state
		AT_CHECK_GFX_INFO(Graphics::GetDevice()->CreateRasterizerState(&desc, &m_Rasteriser));
	}

	std::shared_ptr<Rasteriser> Rasteriser::Create(bool antialiedLine, bool multiSampling, D3D11_CULL_MODE cullMode, D3D11_FILL_MODE fillMode, bool frontAnticlockwise, int depthBias, float DepthBiasClamp, float SlopeScaledDepthBias, bool enableDepthClip, bool enableScissorCulling)
	{
		//Get the UID and get the pointer to the data
		std::string UID = GenerateUID(antialiedLine, multiSampling, cullMode, fillMode, frontAnticlockwise, 
			depthBias, DepthBiasClamp, SlopeScaledDepthBias, enableDepthClip, enableScissorCulling);

		auto test = BindableLib::Resolve(UID);

		//If it isn't nullptr, cast it and return it
		if (test)
		{
			return std::static_pointer_cast<Rasteriser>(test);
		}
		//else create a rasteriser and add it to the library before returning it
		else
		{
			auto rasteriser = new Rasteriser(antialiedLine, multiSampling, cullMode, fillMode, frontAnticlockwise,
				depthBias, DepthBiasClamp, SlopeScaledDepthBias, enableDepthClip, enableScissorCulling);
			BindableLib::Add(UID, std::shared_ptr<Rasteriser>(std::move(rasteriser)));
			return std::static_pointer_cast<Rasteriser>(BindableLib::Resolve(UID));
		}
	}

	std::string Rasteriser::GenerateUID(bool antialiedLine, bool multiSampling, D3D11_CULL_MODE cullMode, D3D11_FILL_MODE fillMode, bool frontAnticlockwise, int depthBias, float DepthBiasClamp, float SlopeScaledDepthBias, bool enableDepthClip, bool enableScissorCulling)
	{
		//Creates the unique identifier for the class
		return std::string(typeid(Rasteriser).name()) + '_' + std::to_string(fillMode)
			+ '_' + std::to_string(cullMode) + '_' + std::to_string(frontAnticlockwise)
			+ '_' + std::to_string(depthBias) + '_' + std::to_string(DepthBiasClamp)
			+ '_' + std::to_string(SlopeScaledDepthBias) + '_' + std::to_string(enableDepthClip)
			+ '_' + std::to_string(enableScissorCulling) + '_' + std::to_string(multiSampling)
			+ '_' + std::to_string(antialiedLine);
	}

	void Rasteriser::ImmidiateBind()
	{
		AT_CHECK_GFX_INFO_VOID(Graphics::GetContext()->RSSetState(m_Rasteriser.Get()));
	}
	
	void Rasteriser::Bind(wrl::ComPtr<ID3D11DeviceContext> context)
	{
		AT_CHECK_GFX_INFO_VOID(context->RSSetState(m_Rasteriser.Get()));
	}
}