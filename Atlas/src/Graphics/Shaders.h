#pragma once
#include "Graphics/Graphics.h"
#include <d3dcompiler.h>

#pragma comment(lib, "D3DCompiler.lib")

namespace Atlas
{
	class VertexShader
	{
	public:
		VertexShader(std::string path);

		void Bind();
		void Unbind();
		wrl::ComPtr<ID3D11VertexShader> GetVertexShader();
		wrl::ComPtr<ID3DBlob> GetBlob();
	private:
		wrl::ComPtr<ID3D11VertexShader> m_VertexShader;
		wrl::ComPtr<ID3DBlob> m_Blob;
	};

	class PixelShader
	{
	public:
		PixelShader(std::string path);

		void Bind();
		void Unbind();
		wrl::ComPtr<ID3D11PixelShader> GetPixelShader();
		wrl::ComPtr<ID3DBlob> GetBlob();
	private:
		wrl::ComPtr<ID3D11PixelShader> m_PixelShader;
		wrl::ComPtr<ID3DBlob> m_Blob;
	};
}