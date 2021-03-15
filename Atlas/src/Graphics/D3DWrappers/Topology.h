#pragma once
#include "Graphics/Bindable.h"
#include "Graphics/Graphics.h"

namespace Atlas
{
	class Topology : public Bindable
	{
	public:
		static std::shared_ptr<Topology> Create(D3D11_PRIMITIVE_TOPOLOGY topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		static std::string GenerateUID(D3D11_PRIMITIVE_TOPOLOGY topology);

		void ImmediateBind() override;
		void Bind(wrl::ComPtr<ID3D11DeviceContext> context) override;
	private:
		Topology(D3D11_PRIMITIVE_TOPOLOGY topology);

		D3D11_PRIMITIVE_TOPOLOGY m_Topology;
	};
}