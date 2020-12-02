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
		void Execute(wrl::ComPtr<ID3D11DeviceContext> context) override;
		void ExecuteImmidiate() override;
		void Reset() override { m_Jobs.clear(); }
	protected:
		std::vector<Job> m_Jobs;
	};
}