#pragma once
#include "Graphics/Graphics.h"
#include "Graphics/D3DWrappers/Shaders.h"
#include "Graphics/Bindable.h"

namespace Atlas
{
	class InputElement
	{
	public:
		InputElement(std::string semanticName, uint type)
			: m_SemanticName(semanticName), m_Type(type) {}

		std::string& GetSemanticName() { return m_SemanticName; }
		uint GetType() { return m_Type; }

	private:
		std::string m_SemanticName;
		uint m_Type;
	};

	class InputLayout : public Bindable
	{
	public:
		InputLayout(std::vector<InputElement> layout, wrl::ComPtr<ID3DBlob> vertexBufferBlob);
		static std::shared_ptr<InputLayout> Create(std::vector<InputElement> layout, wrl::ComPtr<ID3DBlob> vertexBufferBlob);
		static std::string GenerateUID(std::string layoutNames);

		void Bind() override;
		wrl::ComPtr<ID3D11InputLayout> GetInputLayout();
	private:
		wrl::ComPtr<ID3D11InputLayout> m_InputLayout;
	};
}