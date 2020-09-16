#pragma once
#include <DirectXMath.h>
#include "Graphics/Graphics.h"
#include "Graphics/Bindable.h"
#include "Graphics/D3DWrappers/Buffers.h"

namespace Atlas
{
	class Drawable
	{
	public:
		Drawable() = default;
		Drawable(const Drawable&) = delete;
		virtual ~Drawable() = default;

		void Draw() const;
		void AddBindable(Bindable& bindable);

		virtual DirectX::XMMATRIX GetTransformXM() = 0;
		virtual void Update(float timeStep) = 0;
	private:
		std::vector<Bindable*> m_Bindables;
		const IndexBuffer* m_IndexBuffer = nullptr;
	};
}