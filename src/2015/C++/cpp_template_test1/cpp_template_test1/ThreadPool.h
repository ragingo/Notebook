//#pragma once
//#include <memory>
//#include <vector>
//#include <deque>
//#include <thread>
//#include <condition_variable>
//#include <mutex>
//
//class Delegate;
//
//class ThreadPool
//{
//public:
//	static ThreadPool& GetInstance();
//
//	template<typename...ARGS>
//	static void RegisterTask(Action<ARGS...> task)
//	{
//		std::lock_guard<std::mutex> lock(s_Mutex);
//
//		s_Tasks.push_back(std::shared_ptr<Delegate>(new Action<>(task)));
//	}
//
//	static void SetMaxThreadCount(const size_t& count)
//	{
//		s_MaxThreadCount = count;
//	}
//
//	static const size_t MAX_THREAD_COUNT = 4;
//
//	static void Initialize();
//
//	static void Work();
//
//	~ThreadPool();
//
//private:
//	ThreadPool();
//	ThreadPool(const ThreadPool& other){}
//	ThreadPool& operator=(const ThreadPool& other){}
//
//	static std::vector<std::shared_ptr<std::thread>> s_Threads;
//	static std::deque<std::shared_ptr<Delegate>> s_Tasks;
//	static size_t s_MaxThreadCount;
//	static std::mutex s_Mutex;
//};
//
