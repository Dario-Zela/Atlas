#pragma once
#include "Graphics/Graphics.h"
#include "Graphics/Bindable.h"

namespace Atlas
{
	class DepthStencilBuffer;
	class RenderTarget;

	//A wrapper over the d3d11 texture2d
	class Texture : public Bindable
	{
		friend DepthStencilBuffer;
		friend RenderTarget;
	public:
		//The wrapper over the constructor to get a shared ptr
		static std::shared_ptr<Texture> Create(const std::string& path, bool mipMapping, uint targets = 1 << 1, uint slot = 0);
		static std::shared_ptr<Texture> Create(uint width, uint height, void* data, uint targets = 1 << 1, uint slot = 0);

		//Generates the unique identifier for the texture
		static std::string GenerateUID(const std::string& path, uint slot, bool mipMapping);

		void ImmediateBind() override;	//Binds the texture
		void Bind(wrl::ComPtr<ID3D11DeviceContext> context) override;
	private:
		//This is a constructor on a raw texture
		Texture(ID3D11ShaderResourceView* texture, uint slot, uint targets);

		//Constructor for a texture
		//Takes the path of the texture, whether or not it has mip mapping and it's slot
		Texture(const std::string& path, bool mipMapping, uint targets, uint slot);

		//This is a raw constructor, where you can put set the raw data
		Texture(uint width, uint height, void* data, uint targets, uint slot);

		std::vector<std::function<void(ID3D11DeviceContext*, UINT, UINT, ID3D11ShaderResourceView* const*)>> m_Binds;
		wrl::ComPtr<ID3D11ShaderResourceView> m_Texture;
		uint m_Slot;
	};
}