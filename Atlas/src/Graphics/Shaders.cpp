#include "pch.h"
#include "Shaders.h"

namespace Atlas
{
	//Vertex Shader
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	VertexShader::VertexShader(std::string path)
	{
		//This converts the easier to call string format into a const wchar_t array
		//As it is what is used in the D3DReadFileToBlob function

		std::wstring widePath = L"";
		for (char c : path)
			widePath += (wchar_t)c;

		//Reads the .cso objects and constructs the blob
		AT_CHECK_GFX_INFO(D3DReadFileToBlob(widePath.c_str(), &m_Blob))
		
		//Converts the blob into a vertex shader
		AT_CHECK_GFX_INFO(Graphics::GetDevice()->CreateVertexShader(m_Blob->GetBufferPointer(), m_Blob->GetBufferSize(), nullptr, &m_VertexShader))
	}

	void VertexShader::Bind()
	{
		//Binds it
		AT_CHECK_GFX_INFO_VOID(Graphics::GetContext()->VSSetShader(m_VertexShader.Get(), nullptr, 0))
	}

	void VertexShader::Unbind()
	{
		//Unbinds it
		AT_CHECK_GFX_INFO_VOID(Graphics::GetContext()->VSSetShader(nullptr, nullptr, 0))
	}

	wrl::ComPtr<ID3D11VertexShader> VertexShader::GetVertexShader()
	{
		return m_VertexShader;
	}

	wrl::ComPtr<ID3DBlob> VertexShader::GetBlob()
	{
		return m_Blob;
	}

	//Pixel Shader
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	PixelShader::PixelShader(std::string path)
	{
		//This converts the easier to call string format into a const wchar_t array
		//As it is what is used in the D3DReadFileToBlob function

		std::wstring widePath = L"";
		for (char c : path)
			widePath += (wchar_t)c;

		//Reads the .cso objects and constructs the blob
		AT_CHECK_GFX_INFO(D3DReadFileToBlob(widePath.c_str(), &m_Blob))

		//Converts the blob into a vertex shader
		AT_CHECK_GFX_INFO(Graphics::GetDevice()->CreatePixelShader(m_Blob->GetBufferPointer(), m_Blob->GetBufferSize(), nullptr, &m_PixelShader))
	}

	void PixelShader::Bind()
	{
		//Binds it
		AT_CHECK_GFX_INFO_VOID(Graphics::GetContext()->PSSetShader(m_PixelShader.Get(), nullptr, 0))
	}

	void PixelShader::Unbind()
	{
		//Unbinds it
		AT_CHECK_GFX_INFO_VOID(Graphics::GetContext()->PSSetShader(nullptr, nullptr, 0))
	}

	wrl::ComPtr<ID3D11PixelShader> PixelShader::GetPixelShader()
	{
		return m_PixelShader;
	}

	wrl::ComPtr<ID3DBlob> PixelShader::GetBlob()
	{
		return m_Blob;
	}
}