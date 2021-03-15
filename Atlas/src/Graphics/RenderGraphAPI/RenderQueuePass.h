#pragma once
#include "Graphics/RenderGraphAPI/Pass.h"
#include "Graphics/RenderGraphAPI/Job.h"

namespace Atlas
{
	//A special type of pass that will allow drawing objects
	//It needs to be inherited
	class RenderQueuePass : public Pass
	{
	public:
		//Constructs a render queue with an appropriate name
		RenderQueuePass(const std::string& name);
		//Adds a job to the job queue
		void Accept(Job job);

		//Executes all jobs on a deferred render context
		//Should be inherited for custom executions
		void Execute(wrl::ComPtr<ID3D11DeviceContext> context) override;
		//Executes all jobs on an immediate render context
		//Should be inherited for custom executions
		void ExecuteImmediate() override;
		
		//Clears the job queue
		void Reset() override { m_Jobs.clear(); }
	protected:
		//The job queue
		std::vector<Job> m_Jobs;
	};
}