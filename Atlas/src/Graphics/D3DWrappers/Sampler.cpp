#include "pch.h"
#include "Sampler.h"
#include "Graphics/DxgiInfoManager.h"
#include "Graphics/BindableLib.h"

namespace Atlas
{
    Sampler::Sampler(int slot, bool mipMapping, bool Anisotropy, uint maxAnisotropy)
        : m_Slot(slot)
    {
        D3D11_SAMPLER_DESC desc = {};
        desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
        desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
        desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
        if (Anisotropy) 
        {
            desc.Filter = D3D11_FILTER_ANISOTROPIC;
            AT_ASSERT(maxAnisotropy < D3D11_MAX_MAXANISOTROPY && maxAnisotropy > 0, "Your level of Anisotropy is too high for your GPU, scale it down")
            desc.MaxAnisotropy = maxAnisotropy;
        } 
        else
        {
            desc.Filter = D3D11_FILTER_ANISOTROPIC;
        }
        
        if (mipMapping) 
        {
            desc.MipLODBias = 0;
            desc.MaxLOD = D3D11_FLOAT32_MAX;
            desc.MinLOD = 0;
        }

        AT_CHECK_GFX_INFO(Graphics::GetDevice()->CreateSamplerState(&desc, &m_Sampler));
    }

    std::shared_ptr<Sampler> Sampler::Create(bool mipMapping, bool Anisotropy, uint maxAnisotropy, int slot)
    {
        std::string UID = GenerateUID(slot, mipMapping, Anisotropy, maxAnisotropy);
        auto test = BindableLib::Resolve(UID);

        if (test)
        {
            return std::static_pointer_cast<Sampler>(test);
        }
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
        AT_CHECK_GFX_INFO_VOID(Graphics::GetContext()->PSSetSamplers(m_Slot, 1, m_Sampler.GetAddressOf()));
    }
}