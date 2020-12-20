#pragma once
#include "Graphics/Graphics.h"
#include "Graphics/Bindable.h"

namespace Atlas
{
	class DepthStencilBuffer;

	//A wrapper over the d3d11 texture2d
	class Texture : public Bindable
	{
		friend DepthStencilBuffer;
	public:
		//Constructor for a texture
		//Takes the path of the texture, whether or not it has mipmapping and it's slot
		Texture(std::string path, bool mipMapping, uint targets, uint slot);

		//This is a raw constructor, where you can put set the raw data
		Texture(uint width, uint height, void* data, uint targets, uint slot);

		//This is a constructor on a raw texture
		Texture(ID3D11Texture2D* texture, uint slot);

		//The wrapper over the constructor to get a shared ptr
		static std::shared_ptr<Texture> Create(std::string path, bool mipMapping, uint targets = 1 << 1, uint slot = 0);
		static std::shared_ptr<Texture> Create(uint width, uint height, void* data, uint targets = 1 << 1, uint slot = 0);

		//Generates the unique identifier for the texture
		static std::string GenerateUID(std::string path, uint slot, bool mipMapping);

		void ImmidiateBind() override;	//Binds the texture
		void Bind(wrl::ComPtr<ID3D11DeviceContext> context) override;
	private:
		std::vector<std::function<void(ID3D11DeviceContext*, UINT, UINT, ID3D11ShaderResourceView* const*)>> m_Binds;
		wrl::ComPtr<ID3D11ShaderResourceView> m_Texture;
		uint m_Slot;
	};
}