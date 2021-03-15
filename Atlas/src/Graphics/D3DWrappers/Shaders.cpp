#include "pch.h"
#include "Shaders.h"
#include "Graphics/BindableLib.h"
#include "Graphics/DxgiInfoManager.h"

namespace Atlas
{
	//Vertex Shader
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	VertexShader::VertexShader(const std::string& path)
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

	std::shared_ptr<VertexShader> VertexShader::Create(const std::string& path)
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
			auto vertexShader = new VertexShader(path);
			BindableLib::Add(UID, std::shared_ptr<VertexShader>(std::move(vertexShader)));
			return std::static_pointer_cast<VertexShader>(BindableLib::Resolve(UID));
		}
	}

	std::string VertexShader::GenerateUID(const std::string& path)
	{
		return std::string(typeid(VertexShader).name()) + '_' + path;
	}

	void VertexShader::ImmediateBind()
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

	PixelShader::PixelShader(const std::string& path)
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

	std::shared_ptr<PixelShader> PixelShader::Create(const std::string& path)
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
			auto pixelShader = new PixelShader(path);
			BindableLib::Add(UID, std::shared_ptr<PixelShader>(std::move(pixelShader)));
			return std::static_pointer_cast<PixelShader>(BindableLib::Resolve(UID));
		}
	}

	std::string PixelShader::GenerateUID(const std::string& path)
	{
		return std::string(typeid(PixelShader).name()) + '_' + path;
	}

	void PixelShader::ImmediateBind()
	{
		//Binds the pixel shader
		AT_CHECK_GFX_INFO_VOID(Graphics::GetContext()->PSSetShader(m_PixelShader.Get(), nullptr, 0))
	}

	void PixelShader::Bind(wrl::ComPtr<ID3D11DeviceContext> context)
	{
		//Binds the pixel shader
		AT_CHECK_GFX_INFO_VOID(context->PSSetShader(m_PixelShader.Get(), nullptr, 0))
	}

	//Domain Shader
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	DomainShader::DomainShader(const std::string& path)
	{
		//This converts the easier to call string format into a const wchar_t array
		//As it is what is used in the D3DReadFileToBlob function

		std::wstring widePath = L"";
		for (char c : path)
			widePath += (wchar_t)c;

		//Creates the blob and then uses it to create the shader element
		AT_CHECK_GFX_INFO(D3DReadFileToBlob(widePath.c_str(), &m_Blob))
			AT_CHECK_GFX_INFO(Graphics::GetDevice()->CreateDomainShader(m_Blob->GetBufferPointer(), m_Blob->GetBufferSize(), nullptr, &m_DomainShader))
	}

	std::shared_ptr<DomainShader> DomainShader::Create(const std::string& path)
	{
		//Get the UID and get the pointer to the data
		std::string UID = GenerateUID(path);
		auto test = BindableLib::Resolve(UID);

		//If it isn't nullptr, cast it and return it
		if (test)
		{
			return std::static_pointer_cast<DomainShader>(test);
		}
		//else create a shader and add it to the library before returning it
		else
		{
			auto domainShader = new DomainShader(path);
			BindableLib::Add(UID, std::shared_ptr<DomainShader>(std::move(domainShader)));
			return std::static_pointer_cast<DomainShader>(BindableLib::Resolve(UID));
		}
	}

	std::string DomainShader::GenerateUID(const std::string& path)
	{
		return std::string(typeid(DomainShader).name()) + '_' + path;
	}

	void DomainShader::ImmediateBind()
	{
		//Binds the pixel shader
		AT_CHECK_GFX_INFO_VOID(Graphics::GetContext()->DSSetShader(m_DomainShader.Get(), nullptr, 0))
	}

	void DomainShader::Bind(wrl::ComPtr<ID3D11DeviceContext> context)
	{
		//Binds the pixel shader
		AT_CHECK_GFX_INFO_VOID(context->DSSetShader(m_DomainShader.Get(), nullptr, 0))
	}

	//Hull Shader
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	HullShader::HullShader(const std::string& path)
	{
		//This converts the easier to call string format into a const wchar_t array
//As it is what is used in the D3DReadFileToBlob function

		std::wstring widePath = L"";
		for (char c : path)
			widePath += (wchar_t)c;

		//Creates the blob and then uses it to create the shader element
		AT_CHECK_GFX_INFO(D3DReadFileToBlob(widePath.c_str(), &m_Blob))
			AT_CHECK_GFX_INFO(Graphics::GetDevice()->CreateHullShader(m_Blob->GetBufferPointer(), m_Blob->GetBufferSize(), nullptr, &m_HullShader))
	}

	std::shared_ptr<HullShader> HullShader::Create(const std::string& path)
	{
		//Get the UID and get the pointer to the data
		std::string UID = GenerateUID(path);
		auto test = BindableLib::Resolve(UID);

		//If it isn't nullptr, cast it and return it
		if (test)
		{
			return std::static_pointer_cast<HullShader>(test);
		}
		//else create a shader and add it to the library before returning it
		else
		{
			auto hullShader = new HullShader(path);
			BindableLib::Add(UID, std::shared_ptr<HullShader>(std::move(hullShader)));
			return std::static_pointer_cast<HullShader>(BindableLib::Resolve(UID));
		}
	}

	std::string HullShader::GenerateUID(const std::string& path)
	{
		return std::string();
	}

	void HullShader::ImmediateBind()
	{
		//Binds the pixel shader
		AT_CHECK_GFX_INFO_VOID(Graphics::GetContext()->HSSetShader(m_HullShader.Get(), nullptr, 0))
	}

	void HullShader::Bind(wrl::ComPtr<ID3D11DeviceContext> context)
	{
		//Binds the pixel shader
		AT_CHECK_GFX_INFO_VOID(context->HSSetShader(m_HullShader.Get(), nullptr, 0))
	}

	//Geometry Shader
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	GeometryShader::GeometryShader(const std::string& path)
	{
		//This converts the easier to call string format into a const wchar_t array
//As it is what is used in the D3DReadFileToBlob function

		std::wstring widePath = L"";
		for (char c : path)
			widePath += (wchar_t)c;

		//Creates the blob and then uses it to create the shader element
		AT_CHECK_GFX_INFO(D3DReadFileToBlob(widePath.c_str(), &m_Blob))
			AT_CHECK_GFX_INFO(Graphics::GetDevice()->CreateGeometryShader(m_Blob->GetBufferPointer(), m_Blob->GetBufferSize(), nullptr, &m_GeometryShader))
	}

	std::shared_ptr<GeometryShader> GeometryShader::Create(const std::string& path)
	{
		//Get the UID and get the pointer to the data
		std::string UID = GenerateUID(path);
		auto test = BindableLib::Resolve(UID);

		//If it isn't nullptr, cast it and return it
		if (test)
		{
			return std::static_pointer_cast<GeometryShader>(test);
		}
		//else create a shader and add it to the library before returning it
		else
		{
			auto geometryShader = new GeometryShader(path);
			BindableLib::Add(UID, std::shared_ptr<GeometryShader>(std::move(geometryShader)));
			return std::static_pointer_cast<GeometryShader>(BindableLib::Resolve(UID));
		}
	}

	std::string GeometryShader::GenerateUID(const std::string& path)
	{
		return std::string();
	}

	void GeometryShader::ImmediateBind()
	{
		//Binds the pixel shader
		AT_CHECK_GFX_INFO_VOID(Graphics::GetContext()->GSSetShader(m_GeometryShader.Get(), nullptr, 0))
	}

	void GeometryShader::Bind(wrl::ComPtr<ID3D11DeviceContext> context)
	{
		//Binds the pixel shader
		AT_CHECK_GFX_INFO_VOID(context->GSSetShader(m_GeometryShader.Get(), nullptr, 0))
	}

	//Compute Shader
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	ComputeShader::ComputeShader(const std::string& path)
	{
		//This converts the easier to call string format into a const wchar_t array
		//As it is what is used in the D3DReadFileToBlob function

		std::wstring widePath = L"";
		for (char c : path)
			widePath += (wchar_t)c;

		//Creates the blob and then uses it to create the shader element
		AT_CHECK_GFX_INFO(D3DReadFileToBlob(widePath.c_str(), &m_Blob))
			AT_CHECK_GFX_INFO(Graphics::GetDevice()->CreateComputeShader(m_Blob->GetBufferPointer(), m_Blob->GetBufferSize(), nullptr, &m_ComputeShader))
	}

	std::shared_ptr<ComputeShader> ComputeShader::Create(const std::string& path)
	{
		//Get the UID and get the pointer to the data
		std::string UID = GenerateUID(path);
		auto test = BindableLib::Resolve(UID);

		//If it isn't nullptr, cast it and return it
		if (test)
		{
			return std::static_pointer_cast<ComputeShader>(test);
		}
		//else create a shader and add it to the library before returning it
		else
		{
			auto computeShader = new ComputeShader(path);
			BindableLib::Add(UID, std::shared_ptr<ComputeShader>(std::move(computeShader)));
			return std::static_pointer_cast<ComputeShader>(BindableLib::Resolve(UID));
		}
	}

	std::string ComputeShader::GenerateUID(const std::string& path)
	{
		return std::string();
	}

	void ComputeShader::ImmediateBind()
	{
		//Binds the pixel shader
		AT_CHECK_GFX_INFO_VOID(Graphics::GetContext()->CSSetShader(m_ComputeShader.Get(), nullptr, 0))
	}

	void ComputeShader::Bind(wrl::ComPtr<ID3D11DeviceContext> context)
	{
		//Binds the pixel shader
		AT_CHECK_GFX_INFO_VOID(context->CSSetShader(m_ComputeShader.Get(), nullptr, 0))
	}
}