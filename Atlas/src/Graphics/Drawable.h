#pragma once
#include <DirectXMath.h>
#include "Graphics/Graphics.h"
#include "Graphics/Bindable.h"
#include "Graphics/BindableLib.h"
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
		void AddBindable(std::shared_ptr<Bindable> bindable);

		virtual DirectX::XMMATRIX GetTransformXM() = 0;
		virtual void Update(float timeStep) {}
	private:
		std::vector<std::shared_ptr<Bindable>> m_Bindables;
		IndexBuffer* m_IndexBuffer;
	};
}