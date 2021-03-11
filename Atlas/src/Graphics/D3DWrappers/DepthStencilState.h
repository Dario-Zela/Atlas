#pragma once
#include "Graphics\Graphics.h"

namespace Atlas
{
	//Defines the stencil descriptors for a depth stencil state
	struct DepthStencilDesc
	{
		//The operation done when the stencil test fails
		D3D11_STENCIL_OP StencilFailOp = D3D11_STENCIL_OP_KEEP;
		//The operation done when the depth test fails
		D3D11_STENCIL_OP StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		//The operation done when the stencil test passes
		D3D11_STENCIL_OP StencilPassOp = D3D11_STENCIL_OP_KEEP;
		//The stencil test function
		D3D11_COMPARISON_FUNC StencilFunc = D3D11_COMPARISON_ALWAYS;

		//Returns a UID for the depth stencil descriptor
		std::string ToString()
		{
			return '_' + std::to_string(StencilFailOp)
				+ '_' + std::to_string(StencilDepthFailOp)
				+ '_' + std::to_string(StencilPassOp)
				+ '_' + std::to_string(StencilFunc);
		}
	};

	//Determines the depth and stencil tests and how result are used in the output merger stage
	//It allows or masking and creating outlines
	class DepthStencilState : public Bindable
	{
	public:
		//Creates the depth stencil state
		static std::shared_ptr<DepthStencilState> Create(bool DepthEnable, D3D11_DEPTH_WRITE_MASK DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL, D3D11_COMPARISON_FUNC DepthFunc = D3D11_COMPARISON_LESS,
			bool StencilEnable = false, byte StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK, byte StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK,
			DepthStencilDesc FrontFace = DepthStencilDesc(), DepthStencilDesc BackFace = DepthStencilDesc());

		//Gets the UID for the depth stencil state
		static std::string GenerateUID(bool DepthEnable, D3D11_DEPTH_WRITE_MASK DepthWriteMask, D3D11_COMPARISON_FUNC DepthFunc,
			bool StencilEnable, byte StencilReadMask, byte StencilWriteMask, DepthStencilDesc FrontFace, DepthStencilDesc BackFace);

		//Binds the depth stencil state to the immediate context
		void ImmidiateBind();
		//Binds the depth stencil state to the deferred context
		void Bind(wrl::ComPtr<ID3D11DeviceContext> context);
	private:
		//The constructor of the depth stencil state
		DepthStencilState(bool DepthEnable, D3D11_DEPTH_WRITE_MASK DepthWriteMask, D3D11_COMPARISON_FUNC DepthFunc,
			bool StencilEnable, byte StencilReadMask, byte StencilWriteMask, DepthStencilDesc FrontFace, DepthStencilDesc BackFace);

		//The raw depth stencil state
		wrl::ComPtr<ID3D11DepthStencilState> m_DepthStencilState;
	};
}