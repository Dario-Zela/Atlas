#include "pch.h"
#include "Texture.h"
#include <stb_image.h>
#include "Graphics/DxgiInfoManager.h"
#include "Graphics/BindableLib.h"

namespace Atlas 
{
    Texture::Texture(std::string path, bool mipMapping, uint slot)
        : m_Slot(slot)
    {
		int width, height, channels;
        //Set stbi to flip the image vertically
		stbi_set_flip_vertically_on_load(1);

        //Get the data and check that it is valid
		stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 4);     //This enforces RGBA on the image
		AT_CORE_ASSERT(data, "Failed to load texture");

        //Create the descriptor
        D3D11_TEXTURE2D_DESC textureDescriptor = {};
        textureDescriptor.Width = width;
        textureDescriptor.Height = height;
        textureDescriptor.MipLevels = !mipMapping;
        textureDescriptor.ArraySize = 1;
        textureDescriptor.Format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
        textureDescriptor.SampleDesc.Count = 1;
        textureDescriptor.SampleDesc.Quality = 0;
        textureDescriptor.Usage = D3D11_USAGE_DEFAULT;
        textureDescriptor.BindFlags = D3D11_BIND_SHADER_RESOURCE | (mipMapping ? D3D11_BIND_RENDER_TARGET : 0);     //Make sure that only the needed flags are set
        textureDescriptor.CPUAccessFlags = 0;
        textureDescriptor.MiscFlags = (mipMapping ? D3D11_RESOURCE_MISC_GENERATE_MIPS : 0);

        //Create the texture element
        wrl::ComPtr<ID3D11Texture2D> texture;
        AT_CHECK_GFX_INFO(Graphics::GetDevice()->CreateTexture2D(&textureDescriptor, nullptr, &texture));

        //Update the texture element data to allow for later mipmapping generation
        AT_CHECK_GFX_INFO_VOID(Graphics::GetContext()->UpdateSubresource(texture.Get(), 0, nullptr, data, width * 4, 0));

        //Create the desrcriptor for the shader resource view
        D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceDescriptor = {};
        shaderResourceDescriptor.Format = textureDescriptor.Format;
        shaderResourceDescriptor.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        shaderResourceDescriptor.Texture2D.MostDetailedMip = 0;
        shaderResourceDescriptor.Texture2D.MipLevels = (mipMapping ? -1 : 1);

        //Crete the shader resource view
        AT_CHECK_GFX_INFO(Graphics::GetDevice()->CreateShaderResourceView(texture.Get(), &shaderResourceDescriptor, &m_Texture));

        //Generate the necessary mips if the value is selected
        if (mipMapping) { AT_CHECK_GFX_INFO_VOID(Graphics::GetContext()->GenerateMips(m_Texture.Get())); }

        //Free the resources
		stbi_image_free(data);
    }

    Texture::Texture(uint width, uint height, void* data, uint slot)
        : m_Slot(slot)
    {
        //Create a texture descriptor
        D3D11_TEXTURE2D_DESC textureDescriptor = {};
        textureDescriptor.Width = width;
        textureDescriptor.Height = height;
        textureDescriptor.MipLevels = 1;
        textureDescriptor.ArraySize = 1;
        textureDescriptor.Format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
        textureDescriptor.SampleDesc.Count = 1;
        textureDescriptor.SampleDesc.Quality = 0;
        textureDescriptor.Usage = D3D11_USAGE_DEFAULT;
        textureDescriptor.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        textureDescriptor.CPUAccessFlags = 0;
        textureDescriptor.MiscFlags = 0;

        //Create the subrecource data
        D3D11_SUBRESOURCE_DATA subData = {};
        subData.pSysMem = data;
        subData.SysMemPitch = width * sizeof(uint);

        //Create the texture element
        wrl::ComPtr<ID3D11Texture2D> texture;
        AT_CHECK_GFX_INFO(Graphics::GetDevice()->CreateTexture2D(&textureDescriptor, &subData, &texture));

        //Create the desrcriptor for the shader resource view
        D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceDescriptor = {};
        shaderResourceDescriptor.Format = textureDescriptor.Format;
        shaderResourceDescriptor.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        shaderResourceDescriptor.Texture2D.MostDetailedMip = 0;
        shaderResourceDescriptor.Texture2D.MipLevels = 1;

        //Crete the shader resource view
        AT_CHECK_GFX_INFO(Graphics::GetDevice()->CreateShaderResourceView(texture.Get(), &shaderResourceDescriptor, &m_Texture));
    }

    std::shared_ptr<Texture> Texture::Create(std::string path, bool mipMapping, uint slot)
    {
        //Get the UID and get the pointer to the data
        std::string UID = GenerateUID(path + "_" + std::to_string(mipMapping), slot, mipMapping);
        auto test = BindableLib::Resolve(UID);

        //If it isn't nullptr, cast it and return it
        if (test)
        {
            return std::static_pointer_cast<Texture>(test);
        }
        //else create a texture and add it to the library before returning it
        else
        {
            auto vertexShader = std::make_shared<Texture>(path, mipMapping, slot);
            BindableLib::Add(UID, vertexShader);
            return std::static_pointer_cast<Texture>(BindableLib::Resolve(UID));
        }
    }

    std::shared_ptr<Texture> Texture::Create(uint width, uint height, void* data, uint slot)
    {
        //Get the UID and get the pointer to the data
        std::string UID = GenerateUID(std::to_string(*(uint*)data), slot, false);
        auto test = BindableLib::Resolve(UID);

        //If it isn't nullptr, cast it and return it
        if (test)
        {
            return std::static_pointer_cast<Texture>(test);
        }
        //else create a texture and add it to the library before returning it
        else
        {
            auto vertexShader = std::make_shared<Texture>(width, height, data, slot);
            BindableLib::Add(UID, vertexShader);
            return std::static_pointer_cast<Texture>(BindableLib::Resolve(UID));
        }
    }

    std::string Texture::GenerateUID(std::string path, uint slot, bool mipMapping)
    {
        return std::string(typeid(Texture).name()) + '_' + path + '_' + std::to_string(slot) + '_' + std::to_string(mipMapping);
    }

    void Texture::Bind()
    {
        //Binds the texture
        AT_CHECK_GFX_INFO_VOID(Graphics::GetContext()->PSSetShaderResources(m_Slot, 1, m_Texture.GetAddressOf()));
    }
}