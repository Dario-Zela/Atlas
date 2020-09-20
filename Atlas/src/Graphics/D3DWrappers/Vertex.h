#pragma once
#include "Graphics/D3DWrappers/InputLayout.h"

namespace Atlas
{
	enum class ElementType
	{
		Float4D = DXGI_FORMAT_R32G32B32A32_FLOAT,
		Float3D = DXGI_FORMAT_R32G32B32_FLOAT,
		Float2D = DXGI_FORMAT_R32G32_FLOAT,
		Float = DXGI_FORMAT_R32_FLOAT,
		UInt4D = DXGI_FORMAT_R32G32B32A32_UINT,
		UInt3D = DXGI_FORMAT_R32G32B32_UINT,
		UInt2D = DXGI_FORMAT_R32G32_UINT,
		UInt = DXGI_FORMAT_R32_UINT,
		Byte4D = DXGI_FORMAT_R8G8B8A8_UNORM,
		Byte3D = DXGI_FORMAT_B8G8R8X8_UNORM,
		Byte2D = DXGI_FORMAT_R8G8_UNORM,
		Byte = DXGI_FORMAT_R8_UNORM
	};

	static constexpr uint ElementTypeToStride(ElementType type)
	{
		switch (type)
		{
		case ElementType::Float4D: return 128;
		case ElementType::Float3D: return 96;
		case ElementType::Float2D: return 64;
		case ElementType::Float: return 32;
		case ElementType::UInt4D: return 128;
		case ElementType::UInt3D: return 96;
		case ElementType::UInt2D: return 64;
		case ElementType::UInt: return 32;
		case ElementType::Byte4D: return 32;
		case ElementType::Byte3D: return 24;
		case ElementType::Byte2D: return 16;
		case ElementType::Byte: return 8;
		}
	}

	class Vertex
	{
	public:
		Vertex() = default;
		~Vertex() = default;

		void AddPropriety(std::string semanticName, ElementType type);
		operator std::vector<InputElement>() { return m_Layout; }
		uint GetSizeVertex() { return m_Size; }
 	private:
		std::vector<InputElement> m_Layout;
		uint m_Size;
	};

	class VertexData
	{
	public:
		VertexData(Vertex vertex);


	private:
		std::vector<byte> m_Data;
		Vertex m_layout;
	};
}