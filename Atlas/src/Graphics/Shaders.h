#pragma once
#include "Graphics/Graphics.h"
#include <d3dcompiler.h>

#pragma comment(lib, "D3DCompiler.lib")

namespace Atlas
{
	//A wrapper over a vertex shader
	class VertexShader
	{
	public:
		//Get the shader .cso object
		VertexShader(std::string path);

		//Binds it
		void Bind();
		//Unbinds it
		void Unbind();

		//Getters for the shader and the blob
		wrl::ComPtr<ID3D11VertexShader> GetVertexShader();
		wrl::ComPtr<ID3DBlob> GetBlob();
	private:
		wrl::ComPtr<ID3D11VertexShader> m_VertexShader;
		wrl::ComPtr<ID3DBlob> m_Blob;
	};

	//A wrapper over a pixel shader
	class PixelShader
	{
	public:
		//Get the shader .cso object
		PixelShader(std::string path);

		//Binds it
		void Bind();
		//Unbinds it
		void Unbind();

		//Getters for the shader and the blob
		wrl::ComPtr<ID3D11PixelShader> GetPixelShader();
		wrl::ComPtr<ID3DBlob> GetBlob();
	private:
		wrl::ComPtr<ID3D11PixelShader> m_PixelShader;
		wrl::ComPtr<ID3DBlob> m_Blob;
	};
}