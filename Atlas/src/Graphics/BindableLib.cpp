#include "pch.h"
#include "BindableLib.h"

namespace Atlas
{
	std::unordered_map<std::string, std::shared_ptr<Bindable>>* BindableLib::m_Library = nullptr;

	void BindableLib::Init()
	{
		//Allocates the memory
		m_Library = new std::unordered_map<std::string, std::shared_ptr<Bindable>>();
	}

	void BindableLib::Release()
	{
		//Frees the memory
		delete m_Library;
	}

	std::shared_ptr<Bindable> BindableLib::Resolve(const std::string& UID)
	{
		//Find where the UID is
		auto i = m_Library->find(UID);
		//If it is not there return nothing
		if (i == m_Library->end())
		{
			return nullptr;
		}
		//Else return the value
		else
		{
			return std::shared_ptr<Bindable>(i->second);
		}
	}

	void BindableLib::Add(std::string UID, std::shared_ptr<Bindable> bindable)
	{
		//Add the bindable to the library
		(*m_Library)[UID] = std::move(bindable);
	}

	void BindableLib::Flush()
	{
		//Check if any of the items in the library
		for (auto pairs : *m_Library)
		{
			//Only exist in the library
			if (pairs.second.use_count() == 1)
			{
				//If so, delete them
				m_Library->erase(pairs.first);
			}
		}
	}
}