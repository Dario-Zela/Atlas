#include "pch.h"
#include "RenderQueuePass.h"

namespace Atlas
{
	RenderQueuePass::RenderQueuePass(std::string name)
		: Pass(std::move(name))
	{
	}

	void RenderQueuePass::Accept(Job job)
	{
		m_Jobs.push_back(job);
	}

	void RenderQueuePass::Execute(wrl::ComPtr<ID3D11DeviceContext> context)
	{
		BindAll(context);

		for (const auto& job : m_Jobs)
		{
			job.Execute(context);
		}
	}

	void RenderQueuePass::ExecuteImmidiate()
	{
		BindAll();

		for (const auto& job : m_Jobs)
		{
			job.ExecuteImmidiate();
		}
	}
}