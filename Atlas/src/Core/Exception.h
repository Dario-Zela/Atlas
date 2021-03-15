#pragma once

namespace Atlas
{
	//Custom Exception class
	class AtlasException : public std::exception
	{
	public:
		AtlasException(int line, const char* file) noexcept;
		//This is what will be logged
		const char* what() const noexcept override;
		//Type of exception
		virtual const char* GetType() const noexcept;
		//The line the exception occurred on
		int GetLine() const noexcept;
		//The file the exception occurred on
		const std::string& GetFile() const noexcept;
		//A combination of the getTile and getType
		//Results, used in the inherited exceptions
		std::string GetOriginString() const noexcept;
	private:
		int m_Line;
		std::string m_File;
	protected:
		mutable std::string m_Buffer;
	};
}