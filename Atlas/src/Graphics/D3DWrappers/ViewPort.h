#pragma once
#include "Graphics/Graphics.h"
#include "Graphics/Bindable.h"

namespace Atlas
{
	class ViewPort : public Bindable
	{
	public:
		ViewPort() = default;
		void Create(float x, float y, float width, float height, float zMin, float zMax);

		void Bind() override;
		D3D11_VIEWPORT GetViewPort();
	private:
		D3D11_VIEWPORT m_ViewPort;
	};
}