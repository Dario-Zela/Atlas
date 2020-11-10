#pragma once
#include "Graphics/Graphics.h"
#include "Graphics/Bindable.h"
#include <d3dcompiler.h>
#include "Graphics/BindableLib.h"

#pragma comment(lib, "D3DCompiler.lib")

namespace Atlas
{
	class VertexShader : public Bindable
	{
	public:
		VertexShader(std::string path);
		static std::shared_ptr<VertexShader> Create(std::string path);
		static std::string GenerateUID(std::string path);

		void Bind() override;
		wrl::ComPtr<ID3D11VertexShader> GetVertexShader();
		wrl::ComPtr<ID3DBlob> GetBlob();
	private:
		wrl::ComPtr<ID3D11VertexShader> m_VertexShader;
		wrl::ComPtr<ID3DBlob> m_Blob;
	};

	class PixelShader : public Bindable
	{
	public:
		PixelShader(std::string path);
		static std::shared_ptr<PixelShader> Create(std::string path);
		static std::string GenerateUID(std::string path);

		void Bind() override;
		wrl::ComPtr<ID3D11PixelShader> GetPixelShader();
		wrl::ComPtr<ID3DBlob> GetBlob();
	private:
		wrl::ComPtr<ID3D11PixelShader> m_PixelShader;
		wrl::ComPtr<ID3DBlob> m_Blob;
	};
}