#pragma once
#include "pch.h"
#include "Graphics/Bindable.h"

namespace Atlas
{
	class BindableLib
	{
	public:
		static void Init();
		static void Relese();

		static std::shared_ptr<Bindable> Resolve(const std::string& UID);
		static void Add(std::string UID, std::shared_ptr<Bindable> bindable);
	private:
		static std::unordered_map<std::string, std::shared_ptr<Bindable>> Get() { return *m_Library; }
		static std::unordered_map<std::string, std::shared_ptr<Bindable>>* m_Library;
	};
}
