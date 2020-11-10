#pragma once

namespace Atlas
{
	class Bindable
	{
	public:
		virtual void Bind() = 0;
		virtual ~Bindable() = default;
	};

}