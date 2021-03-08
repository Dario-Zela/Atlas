#pragma once
#include "pch.h"
#include "Graphics/Bindable.h"

namespace Atlas
{
	class BindableLib
	{
	public:
		//Initialises the library
		static void Init();
		//Frees the memory
		static void Release();

		//Tries to get the resource
		static std::shared_ptr<Bindable> Resolve(const std::string& UID);
		//Adds the resource to the library
		static void Add(std::string UID, std::shared_ptr<Bindable> bindable);
		//Frees unused memory
		static void Flush();
	private:
		//Debugger only function
		static std::unordered_map<std::string, std::shared_ptr<Bindable>> Get() { return *m_Library; }
		//The library
		static std::unordered_map<std::string, std::shared_ptr<Bindable>>* m_Library;
	};
}