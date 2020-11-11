#pragma once
#include "Graphics/Graphics.h"
#include "Graphics/Bindable.h"

namespace Atlas
{
	//A wrapper over the d3d11 texture2d
	class Texture : public Bindable
	{
	public:
		//Constructor for a texture
		//Takes the path of the texture, whether or not it has mipmapping and it's slot
		Texture(std::string path, bool mipMapping, uint slot);

		//This is a raw constructor, where you can put set the raw data
		Texture(uint width, uint height, void* data, uint slot);

		//The wrapper over the constructor to get a shared ptr
		static std::shared_ptr<Texture> Create(std::string path, bool mipMapping, uint slot = 0);
		static std::shared_ptr<Texture> Create(uint width, uint height, void* data, uint slot = 0);

		//Generates the unique identifier for the texture
		static std::string GenerateUID(std::string path, uint slot, bool mipMapping);

		void Bind();	//Binds the texture
	private:
		wrl::ComPtr<ID3D11ShaderResourceView> m_Texture;
		uint m_Slot;
	};
}