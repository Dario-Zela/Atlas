#pragma once
#include "Graphics/Graphics.h"
#include "Graphics/Bindable.h"

namespace Atlas
{
	class ViewPort : public Bindable
	{
	public:
		ViewPort(float x, float y, float width, float height, float zMin, float zMax);
		static std::shared_ptr<ViewPort> Create(float x, float y, float width, float height, float zMin, float zMax);
		static std::string GenerateUID(uint x, uint y, uint width, uint height, uint zMin, uint zMax);

		void Bind() override;
		D3D11_VIEWPORT GetViewPort();
	private:
		D3D11_VIEWPORT m_ViewPort;
	};
}