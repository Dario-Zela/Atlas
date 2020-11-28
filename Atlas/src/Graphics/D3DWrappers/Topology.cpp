#include "pch.h"
#include "Topology.h"
#include "Graphics/DxgiInfoManager.h"
#include "Graphics/BindableLib.h"

namespace Atlas
{
	Topology::Topology(uint topology)
		: m_Topology((D3D11_PRIMITIVE_TOPOLOGY)topology) { }

	std::shared_ptr<Topology> Topology::Create(uint topology)
	{
		//Get the UID and get the pointer to the data
		std::string UID = GenerateUID((D3D11_PRIMITIVE_TOPOLOGY)topology);
		auto test = BindableLib::Resolve(UID);

		//If it isn't nullptr, cast it and return it
		if (test)
		{
			return std::static_pointer_cast<Topology>(test);
		}
		//else create a vertex buffer and add it to the library before returning it
		else
		{
			auto topologyWrapper = std::make_shared<Topology>(topology);
			BindableLib::Add(UID, topologyWrapper);
			return std::static_pointer_cast<Topology>(BindableLib::Resolve(UID));
		}
	}

	std::string Topology::GenerateUID(D3D11_PRIMITIVE_TOPOLOGY topology)
	{
		return typeid(Topology).name() + '_' + std::to_string(topology);
	}

	void Topology::ImmidiateBind()
	{
		AT_CHECK_GFX_INFO_VOID(Graphics::GetContext()->IASetPrimitiveTopology(m_Topology));
	}

	void Topology::Bind(wrl::ComPtr<ID3D11DeviceContext> context)
	{
		AT_CHECK_GFX_INFO_VOID(context->IASetPrimitiveTopology(m_Topology));
	}
}