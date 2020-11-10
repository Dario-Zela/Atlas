#include "pch.h"
#include "BindableLib.h"

namespace Atlas 
{
    std::unordered_map<std::string, std::shared_ptr<Bindable>>* BindableLib::m_Library = nullptr;

    void BindableLib::Init()
    {
        m_Library = new std::unordered_map<std::string, std::shared_ptr<Bindable>>();
    }

    void BindableLib::Relese()
    {
        delete m_Library;
    }

    std::shared_ptr<Bindable> BindableLib::Resolve(const std::string& UID)
    {
        auto i = m_Library->find(UID);
        if (i == m_Library->end())
        {
            return nullptr;
        }
        else
        {
            return std::shared_ptr<Bindable>(i->second);
        }
    }

    void BindableLib::Add(std::string UID, std::shared_ptr<Bindable> bindable)
    {
        (*m_Library)[UID] = std::move(bindable);
    }

    void BindableLib::Flush()
    {
        for (auto pairs : *m_Library)
        {
            if (pairs.second.use_count() == 1)
            {
                m_Library->erase(pairs.first);
            }
        }
    }
}