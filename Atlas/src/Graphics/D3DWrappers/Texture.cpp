#include "pch.h"
#include "Texture.h"
#include <stb_image.h>
#include "Graphics/DxgiInfoManager.h"
#include "Graphics/BindableLib.h"

namespace Atlas 
{
    Texture::Texture(std::string path)
    {
		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		AT_CORE_ASSERT(data, "Failed to load texture");

        D3D11_TEXTURE2D_DESC desc = {};
        desc.Width = width;
        desc.Height = height;
        desc.MipLevels = 1;
        desc.ArraySize = 1;
        //AT_ASSERT(channels == 4, "Unsupported type, use a 4 channel picture in the format RGBA");
        desc.Format = channels == 4 ? DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM : DXGI_FORMAT::DXGI_FORMAT_B8G8R8X8_UNORM;
        desc.SampleDesc.Count = 1;
        desc.SampleDesc.Quality = 0;
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        desc.CPUAccessFlags = 0;
        desc.MiscFlags = 0;

        D3D11_SUBRESOURCE_DATA subData = {};
        subData.pSysMem = data;
        subData.SysMemPitch = width * sizeof(uint);

        wrl::ComPtr<ID3D11Texture2D> texture;
        AT_CHECK_GFX_INFO(Graphics::GetDevice()->CreateTexture2D(&desc, &subData, &texture));

        D3D11_SHADER_RESOURCE_VIEW_DESC desc2 = {};
        desc2.Format = desc.Format;
        desc2.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        desc2.Texture2D.MostDetailedMip = 0;
        desc2.Texture2D.MipLevels = 1;

        AT_CHECK_GFX_INFO(Graphics::GetDevice()->CreateShaderResourceView(texture.Get(), &desc2, &m_Texture));

		stbi_image_free(data);
    }

    std::shared_ptr<Texture> Texture::Create(std::string path)
    {
        std::string UID = GenerateUID(path);
        auto test = BindableLib::Resolve(UID);

        if (test)
        {
            return std::static_pointer_cast<Texture>(test);
        }
        else
        {
            auto vertexShader = std::make_shared<Texture>(path);
            BindableLib::Add(UID, vertexShader);
            return std::static_pointer_cast<Texture>(BindableLib::Resolve(UID));
        }
    }

    std::string Texture::GenerateUID(std::string path)
    {
        return std::string(typeid(Texture).name()) + '_' + path;
    }

    void Texture::Bind()
    {
        AT_CHECK_GFX_INFO_VOID(Graphics::GetContext()->PSSetShaderResources(0, 1, m_Texture.GetAddressOf()));
    }
}