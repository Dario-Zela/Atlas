#include "pch.h"
#include "InputLayout.h"
#include "Graphics/BindableLib.h"
#include "Graphics/DxgiInfoManager.h"

namespace Atlas
{
	InputLayout::InputLayout(std::vector<InputElement> layout, wrl::ComPtr<ID3DBlob> vertexBufferBlob)
	{
		uint size = (uint)layout.size();

		AT_CORE_ASSERT(size, "The size of the layout must be greater then 0")

		//Create a set of element descriptors from the vector of elements
		D3D11_INPUT_ELEMENT_DESC* elementDesc = new D3D11_INPUT_ELEMENT_DESC[size];
		for (uint i = 0; i < size; i++)
		{
			elementDesc[i] = { layout[i].GetSemanticName().c_str(), 0, (DXGI_FORMAT)layout[i].GetType(), 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 };
		}

		//Create the layout from the elements
		AT_CHECK_GFX_INFO(Graphics::GetDevice()->CreateInputLayout(elementDesc, size, vertexBufferBlob->GetBufferPointer(), vertexBufferBlob->GetBufferSize(), &m_InputLayout));

		//Delete the used resources
		delete[] elementDesc;
	}

	std::shared_ptr<InputLayout> InputLayout::Create(std::vector<InputElement> layout, wrl::ComPtr<ID3DBlob> vertexBufferBlob, const std::string& tag)
	{
		std::string layoutNames = "";
		for (InputElement element : layout)
		{
			layoutNames += element.GetSemanticName();
		}

		//Get the UID and get the pointer to the data
		std::string UID = GenerateUID(layoutNames, tag);
		auto test = BindableLib::Resolve(UID);

		//If it isn't nullptr, cast it and return it
		if (test)
		{
			return std::static_pointer_cast<InputLayout>(test);
		}
		//else create an input layout and add it to the library before returning it
		else
		{
			auto inputLayout = new InputLayout(layout, vertexBufferBlob);
			BindableLib::Add(UID, std::shared_ptr<InputLayout>(std::move(inputLayout)));
			return std::static_pointer_cast<InputLayout>(BindableLib::Resolve(UID));
		}
	}

	std::string InputLayout::GenerateUID(const std::string& layoutNames, const std::string& tag)
	{
		return std::string(typeid(InputLayout).name()) + '_'  + tag + '_' + layoutNames;
	}

	void InputLayout::ImmediateBind()
	{
		//Binds the layout
		AT_CHECK_GFX_INFO_VOID(Graphics::GetContext()->IASetInputLayout(m_InputLayout.Get()));
	}

	void InputLayout::Bind(wrl::ComPtr<ID3D11DeviceContext> context)
	{
		//Binds the layout
		AT_CHECK_GFX_INFO_VOID(context->IASetInputLayout(m_InputLayout.Get()));
	}
}