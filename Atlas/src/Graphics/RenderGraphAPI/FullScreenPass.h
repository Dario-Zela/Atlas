#pragma once
#include "Graphics/RenderGraphAPI/Pass.h"

namespace Atlas
{
	//A special pass that will generate a vertex buffer 
	//and an index buffer over the whole screen
	//This is useful if you want a pass that will display something over
	//the whole window
	//Must be inherited
	class FullScreenPass : public Pass
	{
	public:
		//Makes the pass, vertex buffer and index buffer
		//Requires a name
		FullScreenPass(std::string name);
		
		//Executes a draw call on a deferred render context
		//Should be inherited for custom executions
		void Execute(wrl::ComPtr<ID3D11DeviceContext> context) override;
		//Executes a draw call on an immidiate render context
		//Should be inherited for custom executions
		void ExecuteImmidiate() override;
	};
}