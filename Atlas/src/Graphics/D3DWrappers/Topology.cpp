#include "pch.h"
#include "Topology.h"
#include "Graphics/DxgiInfoManager.h"
#include "Graphics/BindableLib.h"

namespace Atlas
{
	Topology::Topology(D3D11_PRIMITIVE_TOPOLOGY topology)
		: m_Topology(topology) { }

	std::shared_ptr<Topology> Topology::Create(D3D11_PRIMITIVE_TOPOLOGY topology)
	{
		//Get the UID and get the pointer to the data
		std::string UID = GenerateUID(topology);
		auto test = BindableLib::Resolve(UID);

		//If it isn't nullptr, cast it and return it
		if (test)
		{
			return std::static_pointer_cast<Topology>(test);
		}
		//else create a topology and add it to the library before returning it
		else
		{
			auto topologyWrapper = new Topology(topology);
			BindableLib::Add(UID, std::shared_ptr<Topology>(std::move(topologyWrapper)));
			return std::static_pointer_cast<Topology>(BindableLib::Resolve(UID));
		}
	}

	std::string Topology::GenerateUID(D3D11_PRIMITIVE_TOPOLOGY topology)
	{
		return typeid(Topology).name() + '_' + std::to_string(topology);
	}

	void Topology::ImmediateBind()
	{
		AT_CHECK_GFX_INFO_VOID(Graphics::GetContext()->IASetPrimitiveTopology(m_Topology));
	}

	void Topology::Bind(wrl::ComPtr<ID3D11DeviceContext> context)
	{
		AT_CHECK_GFX_INFO_VOID(context->IASetPrimitiveTopology(m_Topology));
	}
}