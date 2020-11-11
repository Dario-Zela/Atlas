#pragma once
#include "Graphics/Graphics.h"
#include "Graphics/D3DWrappers/Shaders.h"
#include "Graphics/Bindable.h"

namespace Atlas
{
	class InputElement
	{
	public:
		//Constructor, takes the semantic name of the element along with its type
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
		//Constructor for the input layout
		//Needs a vector of input elements and the ID3Dblob from the vertex shader
		InputLayout(std::vector<InputElement> layout, wrl::ComPtr<ID3DBlob> vertexBufferBlob);
		
		//The wrapper over the constructor to get a shared ptr
		static std::shared_ptr<InputLayout> Create(std::vector<InputElement> layout, wrl::ComPtr<ID3DBlob> vertexBufferBlob, std::string tag);
		
		//Generates the unique identifier for the input layout
		static std::string GenerateUID(std::string layoutNames, std::string tag);

		void Bind() override;	//Binds the layout
	private:
		wrl::ComPtr<ID3D11InputLayout> m_InputLayout;
	};
}