#pragma once
#include "Graphics/Bindable.h"
#include "Graphics/Graphics.h"

namespace Atlas
{
	class Blendable : public Bindable
	{
	public:
		Blendable(bool blending, int renderTarget);
		static std::shared_ptr<Blendable> Create(bool blending, int renderTarget);

		void Bind() override;
	private:
		wrl::ComPtr<ID3D11BlendState> m_BlendState;
	};
}