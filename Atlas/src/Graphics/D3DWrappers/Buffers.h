#pragma once
#include "Graphics/Graphics.h"
#include "Graphics/Bindable.h"

namespace Atlas
{
	class VertexBuffer : public Bindable
	{
	public:
		VertexBuffer() = default;
		void Create(void* data, uint sizeData, uint sizeVertex);

		void Bind() override;
		wrl::ComPtr<ID3D11Buffer> GetVertexBuffer();
	private:
		wrl::ComPtr<ID3D11Buffer> m_VertexBuffer;
		uint m_Stride;
	};

	class IndexBuffer : public Bindable
	{
	public:
		IndexBuffer() = default;
		void Create(unsigned short* data, uint size);

		void Bind() override;
		wrl::ComPtr<ID3D11Buffer> GetIndexBuffer();
		uint GetCount() const;
	private:
		wrl::ComPtr<ID3D11Buffer> m_IndexBuffer;
	};


	//Constant Buffer types
	////////////////////////////////////////////////////////////////////////////////////////////////////////
	class ConstantBuffer : public Bindable
	{
	public:
		void Create(void* data, uint sizeData);
		void Create(uint sizeData);
		void Update(void* data, uint sizeData);

	protected:
		wrl::ComPtr<ID3D11Buffer> m_ConstantBuffer;
	};

	class VertexConstantBuffer : public ConstantBuffer
	{
	public:
		void Bind() override;
	};

	class PixelConstantBuffer : public ConstantBuffer
	{
	public:
		void Bind() override;
	};
}
