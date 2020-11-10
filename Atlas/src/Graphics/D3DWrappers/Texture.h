#pragma once
#include "Graphics/Graphics.h"
#include "Graphics/Bindable.h"

namespace Atlas
{
	class Texture : public Bindable
	{
	public:
		Texture(std::string path, bool mipMapping, uint slot);
		Texture(uint width, uint height, void* data, uint slot);
		static std::shared_ptr<Texture> Create(std::string path, bool mipMapping, uint slot = 0);
		static std::shared_ptr<Texture> Create(uint width, uint height, void* data, uint slot = 0);
		static std::string GenerateUID(std::string path, uint slot);

		void Bind();
	private:
		wrl::ComPtr<ID3D11ShaderResourceView> m_Texture;
		uint m_Slot;
	};
}