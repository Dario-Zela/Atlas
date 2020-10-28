#pragma once
#include "Graphics/Graphics.h"
#include "Graphics/Bindable.h"

namespace Atlas
{
	class Texture : public Bindable
	{
	public:
		Texture(std::string path, uint slot = 0);
		static std::shared_ptr<Texture> Create(std::string path, uint slot = 0);
		static std::string GenerateUID(std::string path, uint slot = 0);

		void Bind();
	private:
		wrl::ComPtr<ID3D11ShaderResourceView> m_Texture;
		uint m_Slot;
	};
}