#pragma once
#include "Graphics/Bindable.h"
#include "Graphics/Graphics.h"

namespace Atlas
{
	class Topology : public Bindable
	{
	public:
		Topology(uint topology);

		static std::shared_ptr<Topology> Create(uint topology = (uint)D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		static std::string GenerateUID(D3D11_PRIMITIVE_TOPOLOGY topology);

		void Bind();

	private:
		D3D11_PRIMITIVE_TOPOLOGY m_Topology;
	};
}