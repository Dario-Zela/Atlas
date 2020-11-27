#pragma once
#include "Graphics/RenderGraphAPI/Pass.h"

namespace Atlas
{
	class FullScreenPass : public Pass
	{
	public:
		FullScreenPass(std::string name);
		void Execute() override;
	};
}