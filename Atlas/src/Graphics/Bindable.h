#pragma once
#include <d3d11.h>
#include <wrl.h>
namespace wrl = Microsoft::WRL;

namespace Atlas
{
	class Bindable
	{
	public:
		virtual void ImmidiateBind() = 0;
		virtual void Bind(wrl::ComPtr<ID3D11DeviceContext> context) = 0;
		virtual ~Bindable() = default;
	};

}