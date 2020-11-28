#include "pch.h"
#include "Shaders.h"

#include "Graphics/DxgiInfoManager.h"

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

		//Creates the blob and then uses it to create the shader element
		AT_CHECK_GFX_INFO(D3DReadFileToBlob(widePath.c_str(), &m_Blob))
			AT_CHECK_GFX_INFO(Graphics::GetDevice()->CreateVertexShader(m_Blob->GetBufferPointer(), m_Blob->GetBufferSize(), nullptr, &m_VertexShader))
	}

	std::shared_ptr<VertexShader> VertexShader::Create(std::string path)
	{
		//Get the UID and get the pointer to the data
		std::string UID = GenerateUID(path);
		auto test = BindableLib::Resolve(UID);

		//If it isn't nullptr, cast it and return it
		if (test)
		{
			return std::static_pointer_cast<VertexShader>(test);
		}
		//else create a shader and add it to the library before returning it
		else
		{
			auto vertexShader = std::make_shared<VertexShader>(path);
			BindableLib::Add(UID, vertexShader);
			return std::static_pointer_cast<VertexShader>(BindableLib::Resolve(UID));
		}
	}

	std::string VertexShader::GenerateUID(std::string path)
	{
		return std::string(typeid(VertexShader).name()) + '_' + path;
	}

	void VertexShader::ImmidiateBind()
	{
		//Binds the vertex shader
		AT_CHECK_GFX_INFO_VOID(Graphics::GetContext()->VSSetShader(m_VertexShader.Get(), nullptr, 0))
	}

	void VertexShader::Bind(wrl::ComPtr<ID3D11DeviceContext> context)
	{
		//Binds the vertex shader
		AT_CHECK_GFX_INFO_VOID(context->VSSetShader(m_VertexShader.Get(), nullptr, 0))
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

		//Creates the blob and then uses it to create the shader element
		AT_CHECK_GFX_INFO(D3DReadFileToBlob(widePath.c_str(), &m_Blob))
		AT_CHECK_GFX_INFO(Graphics::GetDevice()->CreatePixelShader(m_Blob->GetBufferPointer(), m_Blob->GetBufferSize(), nullptr, &m_PixelShader))
	}

	std::shared_ptr<PixelShader> PixelShader::Create(std::string path)
	{
		//Get the UID and get the pointer to the data
		std::string UID = GenerateUID(path);
		auto test = BindableLib::Resolve(UID);

		//If it isn't nullptr, cast it and return it
		if (test)
		{
			return std::static_pointer_cast<PixelShader>(test);
		}
		//else create a shader and add it to the library before returning it
		else
		{
			auto pixelShader = std::make_shared<PixelShader>(path);
			BindableLib::Add(UID, pixelShader);
			return std::static_pointer_cast<PixelShader>(BindableLib::Resolve(UID));
		}
	}

	std::string PixelShader::GenerateUID(std::string path)
	{
		return std::string(typeid(PixelShader).name()) + '_' + path;
	}

	void PixelShader::ImmidiateBind()
	{
		//Binds the pixel shader
		AT_CHECK_GFX_INFO_VOID(Graphics::GetContext()->PSSetShader(m_PixelShader.Get(), nullptr, 0))
	}

	void PixelShader::Bind(wrl::ComPtr<ID3D11DeviceContext> context)
	{
		//Binds the pixel shader
		AT_CHECK_GFX_INFO_VOID(context->PSSetShader(m_PixelShader.Get(), nullptr, 0))
	}
}