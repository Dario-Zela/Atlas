#pragma once
#include "Graphics\Bindable.h"

namespace Atlas
{
	//Defines classes that can be used as sinks and 
	//sources in a render graph
	class Buffer : public Bindable
	{
	public:
		Buffer() = default;
		//Gets the type of the class when it is stored as a buffer
		virtual std::string GetTypeName() = 0;
		//Turns a buffer into the input buffer
		virtual void Equate(std::shared_ptr<Buffer> buffer) = 0;
	};
}