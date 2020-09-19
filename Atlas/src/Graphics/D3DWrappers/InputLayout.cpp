#include "pch.h"
#include "InputLayout.h"
#include "Graphics/BindableLib.h"
#include "Graphics/DxgiInfoManager.h"

namespace Atlas
{
	std::shared_ptr<InputLayout> InputLayout::Create(std::vector<InputElement> layout, wrl::ComPtr<ID3DBlob> vertexBufferBlob)
	{
		std::string layoutNames = "";
		for (InputElement element : layout)
		{
			layoutNames += element.GetSemanticName();
		}

		std::string UID = GenerateUID(layoutNames);
		auto test = BindableLib::Resolve(UID);

		if (test)
		{
			return std::static_pointer_cast<InputLayout>(test);
		}
		else
		{
			auto inputLayout = std::make_shared<InputLayout>(layout, vertexBufferBlob);
			BindableLib::Add(UID, inputLayout);
			return std::static_pointer_cast<InputLayout>(BindableLib::Resolve(UID));
		}
	}

	std::string InputLayout::GenerateUID(std::string layoutNames)
	{
		return std::string(typeid(InputLayout).name()) + '_' + layoutNames;
	}

	void InputLayout::Bind()
	{
		AT_CHECK_GFX_INFO_VOID(Graphics::GetContext()->IASetInputLayout(m_InputLayout.Get()));
	}

	inline wrl::ComPtr<ID3D11InputLayout> InputLayout::GetInputLayout() 
	{ 
		return m_InputLayout;
	}

	InputLayout::InputLayout(std::vector<InputElement> layout, wrl::ComPtr<ID3DBlob> vertexBufferBlob)
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
}