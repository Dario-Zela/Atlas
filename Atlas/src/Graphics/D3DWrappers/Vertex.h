#pragma once
#include "Graphics/D3DWrappers/Buffers.h"
#include "Graphics/D3DWrappers/InputLayout.h"

namespace Atlas
{
	class Vertex
	{
	public:
		Vertex() = default;
		void AddAtribute(std::string name, uint type, int size);
		void AddAtribute(std::tuple<std::string, uint, int> vertexData);
		void PushVertexData(void* data);
		std::shared_ptr<InputLayout> GetInputLayout(wrl::ComPtr<ID3DBlob> blob, std::string tag);
		std::shared_ptr<VertexBuffer> GetVertexBuffer(std::string tag);

	private:
		std::vector<InputElement> m_VectorDescriptor;
		std::vector<byte> m_Data;
		int m_Stride;
	};
}