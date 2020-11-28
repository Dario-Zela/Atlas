#pragma once
#include "Graphics/Graphics.h"
#include "Graphics/Bindable.h"

namespace Atlas
{
	//A wrapper over a d3d11 sampling state
	class Sampler : public Bindable
	{
	public:
		//Constructor, take the slot of the texture, if it has mip mapping and anisotropy, and the max anisotropy
		Sampler(int slot, bool mipMapping, bool Anisotropy, uint maxAnisotropy);

		//The wrapper over the constructor to get a shared ptr
		static std::shared_ptr<Sampler> Create(bool mipMapping, bool Anisotropy, uint maxAnisotropy, int slot = 0);
		//Generates the unique identifier for the sampler
		static std::string GenerateUID(int slot, bool mipMapping, bool Anisotropy, uint maxAnisotropy);

		void ImmidiateBind() override;		//Binds the sampler
		void Bind(wrl::ComPtr<ID3D11DeviceContext> context) override;
	private:
		int m_Slot;
		wrl::ComPtr<ID3D11SamplerState> m_Sampler;
	};
}