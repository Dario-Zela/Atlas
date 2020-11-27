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

	void RenderQueuePass::Execute()
	{
		BindAll();

		for (const auto& job : m_Jobs)
		{
			job.Execute();
		}
	}
}