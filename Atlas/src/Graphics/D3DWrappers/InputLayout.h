#pragma once
#include "Graphics/Graphics.h"
#include "Graphics/D3DWrappers/Shaders.h"
#include "Graphics/Bindable.h"

namespace Atlas
{
	class InputElement
	{
	public:
		InputElement(std::string semanticName, uint type, uint slot)
			: m_SemanticName(semanticName), m_Type(type), m_Slot(slot) {}

		std::string& GetSemanticName() { return m_SemanticName; }
		uint GetType() { return m_Type; }
		uint GetSlot() { return m_Slot; }

	private:
		std::string m_SemanticName;
		uint m_Type;
		uint m_Slot;
	};

	class InputLayout : public Bindable
	{
	public:
		InputLayout() = default;
		void Create(std::vector<InputElement> layout, wrl::ComPtr<ID3DBlob> vertexBufferBlob);

		void Bind() override;
		wrl::ComPtr<ID3D11InputLayout> GetInputLayout();
	private:
		wrl::ComPtr<ID3D11InputLayout> m_InputLayout;
	};
}