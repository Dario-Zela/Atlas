#include "pch.h"
#include "Shaders.h"

#include "Graphics/DxgiInfoManager.h"

namespace Atlas
{
	//Vertex Shader
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	std::shared_ptr<VertexShader> VertexShader::Create(std::string path)
	{
		std::string UID = GenerateUID(path);
		auto test = BindableLib::Resolve(UID);

		if (test)
		{
			return std::static_pointer_cast<VertexShader>(test);
		}
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

	void VertexShader::Bind()
	{
		AT_CHECK_GFX_INFO_VOID(Graphics::GetContext()->VSSetShader(m_VertexShader.Get(), nullptr, 0))
	}

	wrl::ComPtr<ID3D11VertexShader> VertexShader::GetVertexShader()
	{
		return m_VertexShader;
	}

	wrl::ComPtr<ID3DBlob> VertexShader::GetBlob()
	{
		return m_Blob;
	}

	VertexShader::VertexShader(std::string path)
	{
		//This converts the easier to call string format into a const wchar_t array
		//As it is what is used in the D3DReadFileToBlob function

		std::wstring widePath = L"";
		for (char c : path)
			widePath += (wchar_t)c;

		AT_CHECK_GFX_INFO(D3DReadFileToBlob(widePath.c_str(), &m_Blob))
		AT_CHECK_GFX_INFO(Graphics::GetDevice()->CreateVertexShader(m_Blob->GetBufferPointer(), m_Blob->GetBufferSize(), nullptr, &m_VertexShader))
	}

	//Pixel Shader
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	std::shared_ptr<PixelShader> PixelShader::Create(std::string path)
	{
		std::string UID = GenerateUID(path);
		auto test = BindableLib::Resolve(UID);

		if (test)
		{
			return std::static_pointer_cast<PixelShader>(test);
		}
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

	void PixelShader::Bind()
	{
		AT_CHECK_GFX_INFO_VOID(Graphics::GetContext()->PSSetShader(m_PixelShader.Get(), nullptr, 0))
	}

	wrl::ComPtr<ID3D11PixelShader> PixelShader::GetPixelShader()
	{
		return m_PixelShader;
	}

	wrl::ComPtr<ID3DBlob> PixelShader::GetBlob()
	{
		return m_Blob;
	}

	PixelShader::PixelShader(std::string path)
	{
		//This converts the easier to call string format into a const wchar_t array
		//As it is what is used in the D3DReadFileToBlob function

		std::wstring widePath = L"";
		for (char c : path)
			widePath += (wchar_t)c;

		AT_CHECK_GFX_INFO(D3DReadFileToBlob(widePath.c_str(), &m_Blob))
		AT_CHECK_GFX_INFO(Graphics::GetDevice()->CreatePixelShader(m_Blob->GetBufferPointer(), m_Blob->GetBufferSize(), nullptr, &m_PixelShader))
	}
}