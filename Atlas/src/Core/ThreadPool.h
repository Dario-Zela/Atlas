#pragma once
#include <condition_variable>
#include <thread>
#include <mutex>
#include <functional>
#include <queue>
#include <atomic>

namespace Atlas 
{
	class CountedMutex : public std::mutex
	{
	public:
		CountedMutex()
			: std::mutex(), m_Max(0), m_Count(0) { }

		bool IsSynced() { return m_Max == m_Count; }
		bool IsFullyUsed() { return m_Count == 0; }

		void SetMax(uint numberOfThreads) { m_Max = numberOfThreads; }

		void InUse() { m_Count--; }
		void Waiting() { m_Count++; }
	private:
		uint m_Max;
		std::atomic<uint> m_Count;
	};

	class ThreadPool
	{
	public:
		ThreadPool() : m_Alive(false) {}
		~ThreadPool();

		void CreatePool(int numberOfThreads);

		void AddWork(std::function<void(void)> work);
		void Sync();
	private:
		void Thread();

		CountedMutex m_Mutex;
		std::vector<std::thread> m_Threads;
		std::queue<std::function<void(void)>> m_Jobs;
		std::condition_variable m_Condition;
		bool m_Alive;
	};
}