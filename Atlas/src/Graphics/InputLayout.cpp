#include "pch.h"
#include "InputLayout.h"

namespace Atlas
{
	InputLayout::InputLayout(std::vector<InputElement> layout, wrl::ComPtr<ID3DBlob> vertexBufferBlob)
	{
		uint size = (uint)layout.size();

		//Creates a raw element descriptor array
		D3D11_INPUT_ELEMENT_DESC* elementDesc = new D3D11_INPUT_ELEMENT_DESC[size];
		for (uint i = 0; i < size; i++)
		{
			elementDesc[i] = { layout[i].GetSemanticName().c_str(), 0, (DXGI_FORMAT)layout[i].GetType(), layout[i].GetSlot(), D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 };
		}

		//And from it creates the input layout object
		AT_CHECK_GFX_INFO(Graphics::GetDevice()->CreateInputLayout(elementDesc, size, vertexBufferBlob->GetBufferPointer(), vertexBufferBlob->GetBufferSize(), &m_InputLayout));

		//Before deleting all the elements
		delete[] elementDesc;
	}

	void InputLayout::Bind()
	{
		//Binds it
		AT_CHECK_GFX_INFO_VOID(Graphics::GetContext()->IASetInputLayout(m_InputLayout.Get()));
	}

	void InputLayout::Unbind()
	{
		//Unbinds the input layout
		AT_CHECK_GFX_INFO_VOID(Graphics::GetContext()->IASetInputLayout(nullptr));
	}

	inline wrl::ComPtr<ID3D11InputLayout> InputLayout::GetInputLayout() 
	{ 
		return m_InputLayout;
	}
}