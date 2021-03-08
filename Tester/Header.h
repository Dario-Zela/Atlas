#pragma once

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

	void Execute(wrl::ComPtr<ID3D11DeviceContext> context) override
	{
		Atlas::DepthStencilDesc desc;
		desc.StencilFunc = D3D11_COMPARISON_NOT_EQUAL;
		desc.StencilPassOp = D3D11_STENCIL_OP_KEEP;

		auto rt2 = Atlas::DepthStencilState::Create(false, D3D11_DEPTH_WRITE_MASK_ZERO, D3D11_COMPARISON_ALWAYS, true, 0xff, D3D10_DEFAULT_STENCIL_WRITE_MASK, desc);

		rt2->Bind(context);

		BindAll(context);
		
		auto rt = Atlas::RenderTarget::Create(1500, 1000);
		
		camera->Flip();

		rt->Clear();

		rt->Bind(context);

		for (int i = 0; i < m_Jobs.size() - 1; i++)
		{
			m_Jobs[i].Execute(context);
		}
		
		Settle(rt);

		rt2 = Atlas::DepthStencilState::Create(false);

		rt2->Bind(context);

		camera->Flip();

		RenderQueuePass::Execute(context);

		Unsettle();
	}

	void Settle(std::shared_ptr<Atlas::RenderTarget> rt)
	{
		for (auto& job : m_Jobs)
		{
			if (typeid(*job.m_Drawable) == typeid(Screen))
			{
				auto test = (Screen*)job.m_Drawable;
				test->Assign(rt);
			}
		}
	}

	void Unsettle()
	{
		for (auto& job : m_Jobs)
		{
			if (typeid(*job.m_Drawable) == typeid(Screen))
			{
				auto test = (Screen*)job.m_Drawable;
				test->UnAssign();
			}
		}
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
		m_RenderTarget->Clear(0,0,0,1);
		m_DepthBuffer->Clear();
	}

	void ExecuteImmidiate() override
	{
		m_RenderTarget->Clear(0, 0, 0, 1);
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

	void Update()
	{
		m_BackBuffer = Atlas::Graphics::GetRenderTarget();
	}

	void Clear()
	{
		//m_BackBuffer->Clear(0,0,0,1);
		m_DefaultDepth->Clear();
	}
};