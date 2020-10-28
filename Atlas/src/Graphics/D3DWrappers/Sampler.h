#pragma once
#include "Graphics/Graphics.h"
#include "Graphics/Bindable.h"

namespace Atlas
{
	class Sampler : public Bindable
	{
	public:
		Sampler(int slot);
		static std::shared_ptr<Sampler> Create(int slot = 0);
		static std::string GenerateUID(int slot);

		void Bind();
	private:
		int m_Slot;
		wrl::ComPtr<ID3D11SamplerState> m_Sampler;
	};
}