#pragma once
#include "Graphics/Graphics.h"
#include "Graphics/Bindable.h"

namespace Atlas
{
	class Texture : public Bindable
	{
	public:
		Texture(std::string path);
		static std::shared_ptr<Texture> Create(std::string path);
		static std::string GenerateUID(std::string path);

		void Bind();
	private:
		wrl::ComPtr<ID3D11ShaderResourceView> m_Texture;
	};
}