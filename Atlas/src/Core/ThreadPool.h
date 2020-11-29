#pragma once
#include <condition_variable>
#include <thread>
#include <mutex>
#include <functional>
#include <queue>
#include <atomic>
#include "Graphics/RenderGraphAPI/Pass.h"
#include "Graphics/D3DWrappers/DeferredRenderContext.h"

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
		uint m_Max;
	private:
		std::atomic<uint> m_Count;
	};

	class ThreadPool
	{
	public:
		ThreadPool() : m_Alive(false) {}
		~ThreadPool();

		void CreatePool(int numberOfThreads);

		void AddWork(std::function<void(Pass*, wrl::ComPtr<ID3D11DeviceContext>, int)> work, Pass* executable, int threadNum);
		void Sync();
	private:
		void Thread(int index);

		CountedMutex m_Mutex;
		std::vector<std::thread> m_Threads;
		std::queue<std::tuple<std::function<void(Pass*, wrl::ComPtr<ID3D11DeviceContext>, int)>, Pass*, int>> m_Jobs;
		std::condition_variable m_Condition;
		DeferredRenderContext* m_Contexts;
		bool m_Alive;
	};
}