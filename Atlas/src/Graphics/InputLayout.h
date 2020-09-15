#pragma once
#include "Graphics/Graphics.h"
#include "Graphics/Shaders.h"

namespace Atlas
{
	//A wapper over the supported features of elements
	class InputElement
	{
	public:
		InputElement(std::string semanticName, uint type, uint slot)
			: m_SemanticName(semanticName), m_Type(type), m_Slot(slot) {}

		//Getters
		std::string& GetSemanticName() { return m_SemanticName; }
		uint GetType() { return m_Type; }
		uint GetSlot() { return m_Slot; }

	private:
		std::string m_SemanticName;
		uint m_Type;
		uint m_Slot;
	};

	//A wrapper over an input layout element
	class InputLayout
	{
	public:
		InputLayout(std::vector<InputElement> layout, wrl::ComPtr<ID3DBlob> vertexBufferBlob);

		//Binds it
		void Bind();
		//Unbinds it
		void Unbind();
		//Getter
		wrl::ComPtr<ID3D11InputLayout> GetInputLayout();
	private:
		wrl::ComPtr<ID3D11InputLayout> m_InputLayout;
	};
}