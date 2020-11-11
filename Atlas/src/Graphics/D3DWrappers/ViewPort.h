#pragma once
#include "Graphics/Graphics.h"
#include "Graphics/Bindable.h"

namespace Atlas
{
	//Wrapper over a viewport object
	class ViewPort : public Bindable
	{
	public:
		//Constructor, requires the dimensions of the viewport
		ViewPort(float x, float y, float width, float height, float zMin, float zMax);

		//The wrapper over the constructor to get a shared ptr
		static std::shared_ptr<ViewPort> Create(float x, float y, float width, float height, float zMin, float zMax);
		
		//Generates the unique identifier for the input layout
		static std::string GenerateUID(uint x, uint y, uint width, uint height, uint zMin, uint zMax);

		void Bind() override;		//Binds the layout
	private:
		D3D11_VIEWPORT m_ViewPort;
	};
}