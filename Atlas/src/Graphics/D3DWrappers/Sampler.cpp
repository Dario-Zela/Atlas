#include "pch.h"
#include "Sampler.h"
#include "Graphics/DxgiInfoManager.h"
#include "Graphics/BindableLib.h"

namespace Atlas
{
    Sampler::Sampler(int slot, bool mipMapping, bool Anisotropy, uint maxAnisotropy)
        : m_Slot(slot)
    {
        //The descriptor for the sampler 
        D3D11_SAMPLER_DESC samplerDescriptor = {};
        samplerDescriptor.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
        samplerDescriptor.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
        samplerDescriptor.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
        if (Anisotropy) 
        {
            //If it has anisotropy, add anisotropic filtering and set the max anisotropy
            samplerDescriptor.Filter = D3D11_FILTER_ANISOTROPIC;
            //Checks that the max anisotropy is appropriate
            AT_ASSERT(maxAnisotropy < D3D11_MAX_MAXANISOTROPY && maxAnisotropy > 0, "Your level of anisotropy is invalid")
            samplerDescriptor.MaxAnisotropy = maxAnisotropy;
        } 
        else
        {
            //Else use linear mapping
            samplerDescriptor.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
        }
        
        if (mipMapping) 
        {
            //If there is mip mapping add the level of detail
            samplerDescriptor.MipLODBias = 0;
            samplerDescriptor.MaxLOD = D3D11_FLOAT32_MAX;
            samplerDescriptor.MinLOD = 0;
        }

        //Create the sampler element
        AT_CHECK_GFX_INFO(Graphics::GetDevice()->CreateSamplerState(&samplerDescriptor, &m_Sampler));
    }

    std::shared_ptr<Sampler> Sampler::Create(bool mipMapping, bool Anisotropy, uint maxAnisotropy, int slot)
    {
        //Get the UID and get the pointer to the data
        std::string UID = GenerateUID(slot, mipMapping, Anisotropy, maxAnisotropy);
        auto test = BindableLib::Resolve(UID);

        //If it isn't nullptr, cast it and return it
        if (test)
        {
            return std::static_pointer_cast<Sampler>(test);
        }
        //else create a sampler and add it to the library before returning it
        else
        {
            auto vertexShader = std::make_shared<Sampler>(slot, mipMapping, Anisotropy, maxAnisotropy);
            BindableLib::Add(UID, vertexShader);
            return std::static_pointer_cast<Sampler>(BindableLib::Resolve(UID));
        }
    }

    std::string Sampler::GenerateUID(int slot, bool mipMapping, bool Anisotropy, uint maxAnisotropy)
    {
        auto v = std::string(typeid(Sampler).name()) + "_" + std::to_string(mipMapping) + "_" + std::to_string(Anisotropy) + "_" + std::to_string(maxAnisotropy) + "_" + std::to_string(slot);
        return v;
    }

    void Sampler::Bind()
    {
        //Binds the sampler
        AT_CHECK_GFX_INFO_VOID(Graphics::GetContext()->PSSetSamplers(m_Slot, 1, m_Sampler.GetAddressOf()));
    }
}