#pragma once
#include "Graphics/Graphics.h"

namespace Atlas
{
	//A wrapper over the viewport
	class ViewPort
	{
	public:
		//It sets the position that has to be drawn over
		ViewPort(float x, float y, float width, float height, float zMin, float zMax);

		//It binds the view port
		void Bind();
		
		//Allows for the binding of multiple viewports at once
		static void MultiBind(std::vector<ViewPort> viewports);

		//Getter
		D3D11_VIEWPORT GetViewPort();
	private:
		D3D11_VIEWPORT m_ViewPort;
	};
}