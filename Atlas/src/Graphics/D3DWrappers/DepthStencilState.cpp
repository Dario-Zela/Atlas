#include "pch.h"
#include "DepthStencilState.h"
#include "Graphics/DxgiInfoManager.h"
#include "Graphics/BindableLib.h"

namespace Atlas
{
	DepthStencilState::DepthStencilState(bool DepthEnable, D3D11_DEPTH_WRITE_MASK DepthWriteMask, D3D11_COMPARISON_FUNC DepthFunc, 
		bool StencilEnable, byte StencilReadMask, byte StencilWriteMask, DepthStencilDesc FrontFace, DepthStencilDesc BackFace)
	{
		//Create the descriptor
		D3D11_DEPTH_STENCIL_DESC desc = {};
		desc.DepthEnable = DepthEnable;
		desc.DepthWriteMask = DepthWriteMask;
		desc.DepthFunc = DepthFunc;
		desc.StencilEnable = StencilEnable;
		desc.StencilReadMask = StencilReadMask;
		desc.StencilWriteMask = StencilWriteMask;
		desc.FrontFace = *(D3D11_DEPTH_STENCILOP_DESC*)&FrontFace;
		desc.BackFace = *(D3D11_DEPTH_STENCILOP_DESC*)&BackFace;

		//Then use the descriptor to generate a depth stencil state
		AT_CHECK_GFX_INFO(Graphics::GetDevice()->CreateDepthStencilState(&desc, &m_DepthStencilState));
	}

	std::shared_ptr<DepthStencilState> DepthStencilState::Create(bool DepthEnable, D3D11_DEPTH_WRITE_MASK DepthWriteMask, D3D11_COMPARISON_FUNC DepthFunc,
		bool StencilEnable, byte StencilReadMask, byte StencilWriteMask, DepthStencilDesc FrontFace, DepthStencilDesc BackFace)
	{
		//Get the UID and get the pointer to the data
		std::string UID = GenerateUID(DepthEnable, DepthWriteMask, DepthFunc, StencilEnable, StencilReadMask, StencilWriteMask, FrontFace, BackFace);
		auto test = BindableLib::Resolve(UID);

		//If it isn't nullptr, cast it and return it
		if (test)
		{
			return std::static_pointer_cast<DepthStencilState>(test);
		}
		//else create a depth stencil state and add it to the library before returning it
		else
		{
			auto state = new DepthStencilState(DepthEnable, DepthWriteMask, DepthFunc, StencilEnable, StencilReadMask, StencilWriteMask, FrontFace, BackFace);
			BindableLib::Add(UID, std::shared_ptr<DepthStencilState>(std::move(state)));
			return std::static_pointer_cast<DepthStencilState>(BindableLib::Resolve(UID));
		}
	}

	std::string DepthStencilState::GenerateUID(bool DepthEnable, D3D11_DEPTH_WRITE_MASK DepthWriteMask, D3D11_COMPARISON_FUNC DepthFunc, 
		bool StencilEnable, byte StencilReadMask, byte StencilWriteMask, DepthStencilDesc FrontFace, DepthStencilDesc BackFace)
	{
		//Get a unique string describing the entire depth stencil state
		return std::string(typeid(DepthStencilState).name())
			+ '_' + std::to_string(DepthEnable)
			+ '_' + std::to_string(DepthWriteMask)
			+ '_' + std::to_string(DepthFunc)
			+ '_' + std::to_string(StencilEnable)
			+ '_' + std::to_string(StencilReadMask)
			+ '_' + std::to_string(StencilWriteMask)
			+ '_' + FrontFace.ToString()
			+ '_' + BackFace.ToString();
	}

	void DepthStencilState::ImmidiateBind()
	{
		AT_CHECK_GFX_INFO_VOID(Graphics::GetContext()->OMSetDepthStencilState(m_DepthStencilState.Get(), 0xff));
	}

	void DepthStencilState::Bind(wrl::ComPtr<ID3D11DeviceContext> context)
	{
		AT_CHECK_GFX_INFO_VOID(context->OMSetDepthStencilState(m_DepthStencilState.Get(), 0xff));
	}
}