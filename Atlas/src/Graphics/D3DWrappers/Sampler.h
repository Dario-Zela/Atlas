#pragma once
#include "Graphics/Graphics.h"
#include "Graphics/Bindable.h"

namespace Atlas
{
	class Sampler : public Bindable
	{
	public:
		Sampler();
		static std::shared_ptr<Sampler> Create();
		static std::string GenerateUID();

		void Bind();
	private:
		wrl::ComPtr<ID3D11SamplerState> m_Sampler;
	};
}