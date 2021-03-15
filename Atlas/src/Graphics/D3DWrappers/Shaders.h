#pragma once
#include "Graphics/Graphics.h"
#include "Graphics/Bindable.h"
#include <d3dcompiler.h>

#pragma comment(lib, "D3DCompiler.lib")

namespace Atlas
{
	//Wrappers over the d3d11 shaders

	class VertexShader : public Bindable
	{
	public:
		//The wrapper over the constructor to get a shared ptr
		static std::shared_ptr<VertexShader> Create(const std::string& path);

		//Generates the unique identifier for the input layout
		static std::string GenerateUID(const std::string& path);

		void ImmediateBind() override;	//Binds the layout
		void Bind(wrl::ComPtr<ID3D11DeviceContext> context) override;

		//Gets the blob from the vertex shader
		//Required for the input layer
		wrl::ComPtr<ID3DBlob> GetBlob();
	private:
		//Constructor for a vertex shader
		//Takes a path to the cso object
		VertexShader(const std::string& path);

		wrl::ComPtr<ID3D11VertexShader> m_VertexShader;
		wrl::ComPtr<ID3DBlob> m_Blob;
	};

	class PixelShader : public Bindable
	{
	public:
		//The wrapper over the constructor to get a shared ptr
		static std::shared_ptr<PixelShader> Create(const std::string& path);
		
		//Generates the unique identifier for the input layout
		static std::string GenerateUID(const std::string& path);

		void ImmediateBind() override;	//Binds the layout
		void Bind(wrl::ComPtr<ID3D11DeviceContext> context) override;
	private:
		//Constructor for a pixel shader
		//Takes a path to the cso object
		PixelShader(const std::string& path);
		
		wrl::ComPtr<ID3D11PixelShader> m_PixelShader;
		wrl::ComPtr<ID3DBlob> m_Blob;
	};

	class DomainShader : public Bindable
	{
	public:
		//The wrapper over the constructor to get a shared ptr
		static std::shared_ptr<DomainShader> Create(const std::string& path);

		//Generates the unique identifier for the input layout
		static std::string GenerateUID(const std::string& path);

		void ImmediateBind() override;	//Binds the layout
		void Bind(wrl::ComPtr<ID3D11DeviceContext> context) override;
	private:
		//Constructor for a pixel shader
		//Takes a path to the cso object
		DomainShader(const std::string& path);

		wrl::ComPtr<ID3D11DomainShader> m_DomainShader;
		wrl::ComPtr<ID3DBlob> m_Blob;
	};

	class HullShader : public Bindable
	{
	public:
		//The wrapper over the constructor to get a shared ptr
		static std::shared_ptr<HullShader> Create(const std::string& path);

		//Generates the unique identifier for the input layout
		static std::string GenerateUID(const std::string& path);

		void ImmediateBind() override;	//Binds the layout
		void Bind(wrl::ComPtr<ID3D11DeviceContext> context) override;
	private:
		//Constructor for a pixel shader
		//Takes a path to the cso object
		HullShader(const std::string& path);

		wrl::ComPtr<ID3D11HullShader> m_HullShader;
		wrl::ComPtr<ID3DBlob> m_Blob;
	};

	class GeometryShader : public Bindable
	{
	public:
		//The wrapper over the constructor to get a shared ptr
		static std::shared_ptr<GeometryShader> Create(const std::string& path);

		//Generates the unique identifier for the input layout
		static std::string GenerateUID(const std::string& path);

		void ImmediateBind() override;	//Binds the layout
		void Bind(wrl::ComPtr<ID3D11DeviceContext> context) override;
	private:
		//Constructor for a pixel shader
		//Takes a path to the cso object
		GeometryShader(const std::string& path);

		wrl::ComPtr<ID3D11GeometryShader> m_GeometryShader;
		wrl::ComPtr<ID3DBlob> m_Blob;
	};

	class ComputeShader : public Bindable
	{
	public:
		//The wrapper over the constructor to get a shared ptr
		static std::shared_ptr<ComputeShader> Create(const std::string& path);

		//Generates the unique identifier for the input layout
		static std::string GenerateUID(const std::string& path);

		void ImmediateBind() override;	//Binds the layout
		void Bind(wrl::ComPtr<ID3D11DeviceContext> context) override;
	private:
		//Constructor for a pixel shader
		//Takes a path to the cso object
		ComputeShader(const std::string& path);

		wrl::ComPtr<ID3D11ComputeShader> m_ComputeShader;
		wrl::ComPtr<ID3DBlob> m_Blob;
	};
}