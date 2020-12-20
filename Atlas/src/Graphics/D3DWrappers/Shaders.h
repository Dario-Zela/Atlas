#pragma once
#include "Graphics/Graphics.h"
#include "Graphics/Bindable.h"
#include <d3dcompiler.h>
#include "Graphics/BindableLib.h"

#pragma comment(lib, "D3DCompiler.lib")

namespace Atlas
{
	//Wrappers over the d3d11 shaders

	class VertexShader : public Bindable
	{
	public:
		//Constructor for a vertex shader
		//Takes a path to the cso object
		VertexShader(std::string path);

		//The wrapper over the constructor to get a shared ptr
		static std::shared_ptr<VertexShader> Create(std::string path);

		//Generates the unique identifier for the input layout
		static std::string GenerateUID(std::string path);

		void ImmidiateBind() override;	//Binds the layout
		void Bind(wrl::ComPtr<ID3D11DeviceContext> context) override;

		//Gets the blob from the vertex shader
		//Required for the input layer
		wrl::ComPtr<ID3DBlob> GetBlob();
	private:
		wrl::ComPtr<ID3D11VertexShader> m_VertexShader;
		wrl::ComPtr<ID3DBlob> m_Blob;
	};

	class PixelShader : public Bindable
	{
	public:
		//Constructor for a pixel shader
		//Takes a path to the cso object
		PixelShader(std::string path);
		
		//The wrapper over the constructor to get a shared ptr
		static std::shared_ptr<PixelShader> Create(std::string path);
		
		//Generates the unique identifier for the input layout
		static std::string GenerateUID(std::string path);

		void ImmidiateBind() override;	//Binds the layout
		void Bind(wrl::ComPtr<ID3D11DeviceContext> context) override;
	private:
		wrl::ComPtr<ID3D11PixelShader> m_PixelShader;
		wrl::ComPtr<ID3DBlob> m_Blob;
	};

	class DomainShader : public Bindable
	{
	public:
		//Constructor for a pixel shader
		//Takes a path to the cso object
		DomainShader(std::string path);

		//The wrapper over the constructor to get a shared ptr
		static std::shared_ptr<DomainShader> Create(std::string path);

		//Generates the unique identifier for the input layout
		static std::string GenerateUID(std::string path);

		void ImmidiateBind() override;	//Binds the layout
		void Bind(wrl::ComPtr<ID3D11DeviceContext> context) override;
	private:
		wrl::ComPtr<ID3D11DomainShader> m_DomainShader;
		wrl::ComPtr<ID3DBlob> m_Blob;
	};

	class HullShader : public Bindable
	{
	public:
		//Constructor for a pixel shader
		//Takes a path to the cso object
		HullShader(std::string path);

		//The wrapper over the constructor to get a shared ptr
		static std::shared_ptr<HullShader> Create(std::string path);

		//Generates the unique identifier for the input layout
		static std::string GenerateUID(std::string path);

		void ImmidiateBind() override;	//Binds the layout
		void Bind(wrl::ComPtr<ID3D11DeviceContext> context) override;
	private:
		wrl::ComPtr<ID3D11HullShader> m_HullShader;
		wrl::ComPtr<ID3DBlob> m_Blob;
	};

	class GeometryShader : public Bindable
	{
	public:
		//Constructor for a pixel shader
		//Takes a path to the cso object
		GeometryShader(std::string path);

		//The wrapper over the constructor to get a shared ptr
		static std::shared_ptr<GeometryShader> Create(std::string path);

		//Generates the unique identifier for the input layout
		static std::string GenerateUID(std::string path);

		void ImmidiateBind() override;	//Binds the layout
		void Bind(wrl::ComPtr<ID3D11DeviceContext> context) override;
	private:
		wrl::ComPtr<ID3D11GeometryShader> m_GeometryShader;
		wrl::ComPtr<ID3DBlob> m_Blob;
	};

	class ComputeShader : public Bindable
	{
	public:
		//Constructor for a pixel shader
		//Takes a path to the cso object
		ComputeShader(std::string path);

		//The wrapper over the constructor to get a shared ptr
		static std::shared_ptr<ComputeShader> Create(std::string path);

		//Generates the unique identifier for the input layout
		static std::string GenerateUID(std::string path);

		void ImmidiateBind() override;	//Binds the layout
		void Bind(wrl::ComPtr<ID3D11DeviceContext> context) override;
	private:
		wrl::ComPtr<ID3D11ComputeShader> m_ComputeShader;
		wrl::ComPtr<ID3DBlob> m_Blob;
	};
}