#include "pch.h"
#include "ThreadPool.h"

namespace Atlas
{
	void ThreadPool::CreatePool(int numberOfThreads)
	{
		AT_CORE_ASSERT(!m_Alive, "You cannot recrate a thread pool");

		m_Alive = true;
		m_Mutex.SetMax(numberOfThreads);

		m_Threads.reserve(numberOfThreads);
		for (int i = 0; i < numberOfThreads; i++)
			m_Threads.emplace_back(std::bind(&ThreadPool::Thread, this, i));

		m_Contexts = new DeferredRenderContext[numberOfThreads]();
	}

	ThreadPool::~ThreadPool()
	{
		{
			std::unique_lock<std::mutex> lock(m_Mutex);
			m_Alive = false;
			m_Condition.notify_all();
		}

		for (auto& thread : m_Threads)
			thread.join();

		delete[] m_Contexts;
	}

	void ThreadPool::AddWork(std::function<void(Pass*, wrl::ComPtr<ID3D11DeviceContext>, int)> work, Pass* executable, int index)
	{
		while (m_Mutex.IsFullyUsed());

		std::unique_lock<std::mutex> lock(m_Mutex);

		m_Jobs.emplace(std::move(work), executable, index);
		m_Condition.notify_one();
		m_Mutex.InUse();
	}

	void ThreadPool::Sync()
	{
		while (!m_Jobs.empty());
		while (!m_Mutex.IsSynced());
	}

	void ThreadPool::Thread(int threadNum)
	{
		std::function<void(Pass*, wrl::ComPtr<ID3D11DeviceContext>, int)> job;
		Pass* executable = nullptr;
		int index = 0;

		while (true)
		{
			{
				std::unique_lock<std::mutex> lock(m_Mutex);

				m_Mutex.Waiting();
				
				m_Condition.wait(lock);

				if (!m_Alive)
					return;

				auto tuple = std::move(m_Jobs.front());
				job = std::move(std::get<0>(tuple));
				executable = std::get<1>(tuple);
				index = std::get<2>(tuple);
				m_Jobs.pop();
			}
			
			job(executable, m_Contexts[threadNum].GetContext(), index);
		}
	}
}