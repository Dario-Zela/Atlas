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
		//Add the job to the queue
		m_Jobs.push_back(job);
	}

	void RenderQueuePass::Execute(wrl::ComPtr<ID3D11DeviceContext> context)
	{
		//Bind all of the bindables and the 
		//Render target and depth buffer
		BindAll(context);
		
		//Execute every job
		for (const auto& job : m_Jobs)
		{
			job.Execute(context);
		}
	}

	void RenderQueuePass::ExecuteImmidiate()
	{
		//Bind all of the bindables and the 
		//Render target and depth buffer
		BindAll();

		//Execute every job
		for (const auto& job : m_Jobs)
		{
			job.ExecuteImmidiate();
		}
	}
}