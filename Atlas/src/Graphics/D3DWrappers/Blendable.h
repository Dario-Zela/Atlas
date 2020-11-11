#pragma once
#include "Graphics/Bindable.h"
#include "Graphics/Graphics.h"

namespace Atlas
{
	//A wrapper over the blending functions in d3d11
	class Blendable : public Bindable
	{
	public:
		//Constructor takes whether or not there should be blending
		//And the render target
		Blendable(bool blending, int renderTarget);
		static std::shared_ptr<Blendable> Create(bool blending, int renderTarget);	//A static constructor that creates a shared ptr

		void Bind() override;	//Binds the blend function to the pipeline
	private:
		wrl::ComPtr<ID3D11BlendState> m_BlendState;
	};
}