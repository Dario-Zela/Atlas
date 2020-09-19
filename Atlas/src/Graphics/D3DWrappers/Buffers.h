#pragma once
#include "Graphics/Graphics.h"
#include "Graphics/Bindable.h"

namespace Atlas
{
	class VertexBuffer : public Bindable
	{
	public:
		VertexBuffer(void* data, uint sizeData, uint sizeVertex);
		static std::shared_ptr<VertexBuffer> Create(void* data, uint sizeData, uint sizeVertex);

		static std::string GenerateUID(uint stride);
		void Bind() override;
		wrl::ComPtr<ID3D11Buffer> GetVertexBuffer();
	private:
		wrl::ComPtr<ID3D11Buffer> m_VertexBuffer;
		uint m_Stride;
	};

	class IndexBuffer : public Bindable
	{
	public:
		IndexBuffer(unsigned short* data, uint size);
		static std::shared_ptr<IndexBuffer> Create(unsigned short* data, uint size);

		static std::string GenerateUID(uint size);
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
		ConstantBuffer(void* data, uint sizeData);
		ConstantBuffer(uint sizeData);

		static std::shared_ptr<ConstantBuffer> Create(void* data, uint sizeData);
		static std::shared_ptr<ConstantBuffer> Create(uint sizeData);
		void Update(void* data, uint sizeData);
		static std::string GenerateUID(uint sizeData);
		void Bind() override {}
	protected:
		wrl::ComPtr<ID3D11Buffer> m_ConstantBuffer;
	};

	class VertexConstantBuffer : public ConstantBuffer
	{
	public:
		VertexConstantBuffer(void* data, uint sizeData) : ConstantBuffer(data, sizeData) {}
		VertexConstantBuffer(uint sizeData) : ConstantBuffer(sizeData) {}
		void Bind() override;
	};

	class PixelConstantBuffer : public ConstantBuffer
	{
	public:
		PixelConstantBuffer(void* data, uint sizeData) : ConstantBuffer(data, sizeData) {}
		PixelConstantBuffer(uint sizeData) : ConstantBuffer(sizeData) {}
		void Bind() override;
	};
}
