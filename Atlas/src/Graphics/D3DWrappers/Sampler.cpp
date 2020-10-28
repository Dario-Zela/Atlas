#include "pch.h"
#include "Sampler.h"
#include "Graphics/DxgiInfoManager.h"
#include "Graphics/BindableLib.h"

namespace Atlas
{
    Sampler::Sampler(int slot)
        : m_Slot(slot)
    {
        D3D11_SAMPLER_DESC desc = {};
        desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
        desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
        desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
        desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

        AT_CHECK_GFX_INFO(Graphics::GetDevice()->CreateSamplerState(&desc, &m_Sampler));
    }

    std::shared_ptr<Sampler> Sampler::Create(int slot)
    {
        std::string UID = GenerateUID(slot);
        auto test = BindableLib::Resolve(UID);

        if (test)
        {
            return std::static_pointer_cast<Sampler>(test);
        }
        else
        {
            auto vertexShader = std::make_shared<Sampler>(slot);
            BindableLib::Add(UID, vertexShader);
            return std::static_pointer_cast<Sampler>(BindableLib::Resolve(UID));
        }
    }

    std::string Sampler::GenerateUID(int slot)
    {
        return std::string(typeid(Sampler).name()) + "_" + std::to_string(slot);
    }

    void Sampler::Bind()
    {
        AT_CHECK_GFX_INFO_VOID(Graphics::GetContext()->PSSetSamplers(m_Slot, 1, m_Sampler.GetAddressOf()));
    }
}