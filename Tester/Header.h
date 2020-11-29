#pragma once
#include "Atlas.h"

class LambertianPass : public Atlas::RenderQueuePass
{
public:
	LambertianPass()
		: Atlas::RenderQueuePass("LambertianPass")
	{
		RegisterSink(Atlas::Sink::Create("renderTarget", std::static_pointer_cast<Atlas::Buffer>(m_RenderTarget)));
		RegisterSink(Atlas::Sink::Create("depthStencil", std::static_pointer_cast<Atlas::Buffer>(m_DepthBuffer)));
		RegisterSource(Atlas::Source::Create("renderTarget", std::static_pointer_cast<Atlas::Buffer>(m_RenderTarget)));
	}
};

class ClearPass : public Atlas::Pass
{
public:
	ClearPass()
		: Atlas::Pass("ClearPass")
	{
		RegisterSink(Atlas::Sink::Create("renderTarget", std::static_pointer_cast<Atlas::Buffer>(m_RenderTarget)));
		RegisterSink(Atlas::Sink::Create("depthStencil", std::static_pointer_cast<Atlas::Buffer>(m_DepthBuffer)));
		RegisterSource(Atlas::Source::Create("renderTarget", std::static_pointer_cast<Atlas::Buffer>(m_RenderTarget)));
		RegisterSource(Atlas::Source::Create("depthStencil", std::static_pointer_cast<Atlas::Buffer>(m_DepthBuffer)));
	}

	void Execute(wrl::ComPtr<ID3D11DeviceContext> context) override
	{
		m_RenderTarget->Clear(0,0,1,1);
		m_DepthBuffer->Clear();
	}

	void ExecuteImmidiate() override
	{
		m_RenderTarget->Clear(0, 0, 1, 1);
		m_DepthBuffer->Clear();
	}
};

class LambertianRenderGraph : public Atlas::RenderGraph
{
public:
	LambertianRenderGraph()
		: Atlas::RenderGraph()
	{
		{
			auto pass = std::make_unique<ClearPass>();
			pass->SetSinkLink("renderTarget", "$.backBuffer");
			pass->SetSinkLink("depthStencil", "$.defaultDepth");
			AddPass(std::move(pass));
		}
		{
			auto pass = std::make_unique<LambertianPass>();
			pass->SetSinkLink("renderTarget", "ClearPass.renderTarget");
			pass->SetSinkLink("depthStencil", "ClearPass.depthStencil");
			AddPass(std::move(pass));
		}

		SetGlobalSinkTarget("backBuffer", "LambertianPass.renderTarget");
		Finalise();
	}
};

class LambertianRenderGraph2 : public Atlas::RenderGraph
{
public:
	LambertianRenderGraph2()
		: Atlas::RenderGraph()
	{
		{
			auto pass = std::make_unique<LambertianPass>();
			pass->SetSinkLink("renderTarget", "$.backBuffer");
			pass->SetSinkLink("depthStencil", "$.defaultDepth");
			AddPass(std::move(pass));
		}
		SetGlobalSinkTarget("backBuffer", "LambertianPass.renderTarget");
		Finalise();
	}
};