#pragma once
#include "Graphics/Graphics.h"

namespace Atlas
{
	class VertexBuffer
	{
	public:
		VertexBuffer(void* data, uint sizeData, uint sizeVertex);

		void Bind(uint stride, uint offset);
		static void Multibind(std::vector<VertexBuffer> buffers, const uint* strides, const uint* offsets);
		wrl::ComPtr<ID3D11Buffer> GetVertexBuffer();
	private:
		wrl::ComPtr<ID3D11Buffer> m_VertexBuffer;
	};

	class IndexBuffer
	{
	public:
		IndexBuffer(unsigned short* data, uint size);

		void Bind(uint offset);
		static void Multibind(std::vector<IndexBuffer> buffers, const uint* strides, const uint* offsets);
		wrl::ComPtr<ID3D11Buffer> GetIndexBuffer();
		uint GetCount();
	private:
		wrl::ComPtr<ID3D11Buffer> m_IndexBuffer;
	};
}
