#pragma once
#include "Graphics/RenderGraphAPI/Pass.h"
#include "Graphics/RenderGraphAPI/Job.h"

namespace Atlas
{
	class RenderQueuePass : public Pass
	{
	public:
		RenderQueuePass(std::string name);
		void Accept(Job job);
		void Execute() override;
		void Reset() override { m_Jobs.clear(); }
	private:
		std::vector<Job> m_Jobs;
	};
}