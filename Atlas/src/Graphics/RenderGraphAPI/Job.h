#pragma once
#include "Graphics/Drawable.h"
#include "Graphics/RenderGraphAPI/Step.h"

namespace Atlas
{
	class Job
	{
	public:
		Job(const Drawable* drawable, Step* step);
		void Execute(wrl::ComPtr<ID3D11DeviceContext> context) const;
		void ExecuteImmidiate() const;

		const Drawable* m_Drawable;
		Step* m_Step;
	};
}