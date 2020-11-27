#pragma once

namespace Atlas
{
	class Buffer
	{
	public:
		Buffer() = default;
		virtual std::string GetTypeName() = 0;
		virtual void Copy(std::shared_ptr<Buffer> buffer) = 0;
	};
}