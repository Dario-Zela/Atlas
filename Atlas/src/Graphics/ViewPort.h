#pragma once
#include "Graphics/Graphics.h"

namespace Atlas
{
	class ViewPort
	{
	public:
		ViewPort(float x, float y, float width, float height, float zMin, float zMax);

		void Bind();
		static void MultiBind(std::vector<ViewPort> viewports);
		D3D11_VIEWPORT GetViewPort();
	private:
		D3D11_VIEWPORT m_ViewPort;
	};
}