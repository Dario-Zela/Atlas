#include "pch.h"
#include "InputLayout.h"

namespace Atlas
{
	void InputLayout::Create(std::vector<InputElement> layout, wrl::ComPtr<ID3DBlob> vertexBufferBlob)
	{
		uint size = (uint)layout.size();

		D3D11_INPUT_ELEMENT_DESC* elementDesc = new D3D11_INPUT_ELEMENT_DESC[size];
		for (uint i = 0; i < size; i++)
		{
			elementDesc[i] = { layout[i].GetSemanticName().c_str(), 0, (DXGI_FORMAT)layout[i].GetType(), layout[i].GetSlot(), D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 };
		}

		AT_CHECK_GFX_INFO(Graphics::GetDevice()->CreateInputLayout(elementDesc, size, vertexBufferBlob->GetBufferPointer(), vertexBufferBlob->GetBufferSize(), &m_InputLayout));

		delete[] elementDesc;
	}

	void InputLayout::Bind()
	{
		AT_CHECK_GFX_INFO_VOID(Graphics::GetContext()->IASetInputLayout(m_InputLayout.Get()));
	}

	inline wrl::ComPtr<ID3D11InputLayout> InputLayout::GetInputLayout() 
	{ 
		return m_InputLayout;
	}
}