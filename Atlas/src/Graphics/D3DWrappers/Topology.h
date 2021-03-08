#pragma once
#include "Graphics/Bindable.h"
#include "Graphics/Graphics.h"

namespace Atlas
{
	class Topology : public Bindable
	{
	public:
		static std::shared_ptr<Topology> Create(uint topology = (uint)D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		static std::string GenerateUID(D3D11_PRIMITIVE_TOPOLOGY topology);

		void ImmidiateBind() override;
		void Bind(wrl::ComPtr<ID3D11DeviceContext> context) override;
	private:
		Topology(uint topology);

		D3D11_PRIMITIVE_TOPOLOGY m_Topology;
	};
}