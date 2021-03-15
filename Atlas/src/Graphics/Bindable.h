#pragma once
#include <d3d11.h>
#include <wrl.h>
namespace wrl = Microsoft::WRL;

namespace Atlas
{
	class Bindable
	{
	public:
		//Binds the resource to the immediate context
		virtual void ImmediateBind() = 0;
		//Binds the resource to the given context
		virtual void Bind(wrl::ComPtr<ID3D11DeviceContext> context) = 0;
		virtual ~Bindable() = default;
	};

}