#pragma once
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

		void ImmidiateBind() override;		//Binds the layout
		void Bind(wrl::ComPtr<ID3D11DeviceContext> context) override;
	private:
		D3D11_VIEWPORT m_ViewPort;
	};
}