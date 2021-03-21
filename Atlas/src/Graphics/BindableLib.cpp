#include "pch.h"
#include "BindableLib.h"

namespace Atlas
{
	std::unordered_map<std::string, std::shared_ptr<Bindable>>* BindableLib::s_Library = nullptr;

	void BindableLib::Init()
	{
		AT_CORE_ASSERT(!s_Library, "The library already been initialised")

		//Allocates the memory
		s_Library = new std::unordered_map<std::string, std::shared_ptr<Bindable>>();
	}

	void BindableLib::Release()
	{
		AT_CORE_ASSERT(s_Library, "The library not been initialised or it has already been released")

		//Frees the memory
		delete s_Library;
	}

	std::shared_ptr<Bindable> BindableLib::Resolve(const std::string& UID)
	{
		AT_CORE_ASSERT(s_Library, "The library not been initialised or it has already been released")

		//Find where the UID is
		auto i = s_Library->find(UID);
		//If it is not there return nothing
		if (i == s_Library->end())
		{
			return nullptr;
		}
		//Else return the value
		else
		{
			return std::shared_ptr<Bindable>(i->second);
		}
	}

	void BindableLib::Add(const std::string& UID, std::shared_ptr<Bindable> bindable)
	{
		AT_CORE_ASSERT(s_Library, "The library not been initialised or it has already been released")

		//Add the bindables to the library
		(*s_Library)[UID] = std::move(bindable);
	}

	void BindableLib::Flush()
	{
		AT_CORE_ASSERT(s_Library, "The library not been initialised or it has already been released")

		//Add a list of items to delete
		std::vector<std::string> toDelete = std::vector<std::string>();

		//Check if any of the items in the library
		for (auto pairs : *s_Library)
		{
			//Only exist in the library
			if (pairs.second.use_count() == 2)
			{
				//If so, add them to the list
				toDelete.push_back(pairs.first);
			}
		}

		//Then delete them from the list
		for (auto value : toDelete)
		{
			s_Library->erase(value);
		}
	}
}