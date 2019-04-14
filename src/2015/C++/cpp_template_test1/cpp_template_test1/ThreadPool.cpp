//#include "Function.h"
//#include "ThreadPool.h"
//
//std::vector<std::shared_ptr<std::thread>> ThreadPool::s_Threads;
//std::deque<std::shared_ptr<Delegate>> ThreadPool::s_Tasks;
//size_t ThreadPool::s_MaxThreadCount = ThreadPool::MAX_THREAD_COUNT;
//std::mutex ThreadPool::s_Mutex;
//
//
//static std::condition_variable s_Cond;
//
//static std::thread s_ParentThread;
//
//ThreadPool::ThreadPool()
//{
//
//}
//
//ThreadPool::~ThreadPool()
//{
//
//}
//
//ThreadPool& ThreadPool::GetInstance()
//{
//	static ThreadPool instance;
//	return instance;
//}
//
//void ThreadPool::Initialize()
//{
//	for (size_t i = 0; i < s_MaxThreadCount; i++)
//	{
//		s_Threads.push_back(nullptr);
//	}
//	s_ParentThread = std::thread(std::bind(ThreadPool::Work));
//}
//
//void ThreadPool::Work()
//{
//	while (true)
//	{
//		std::this_thread::sleep_for(std::chrono::milliseconds(1));
//
//		if (s_Tasks.empty())
//		{
//			continue;
//		}
//		auto task = s_Tasks.front();
//		if (!task)
//		{
//			continue;
//		}
//		
//		if (task->GetID() != Delegate::OT_Action)
//		{
//			s_Tasks.pop_front();
//			continue;
//		}
//		if (task->HasArguments())
//		{
//			s_Tasks.pop_front();
//			continue;
//		}
//		
//		auto fun = reinterpret_cast<Action<>*>(task.get());
//
//		for (size_t i = 0; i < s_Threads.size(); i++)
//		{
//			if (s_Threads[i] && !s_Threads[i]->joinable())
//			{
//				s_Threads[i] = nullptr;
//			}
//			if (s_Threads[i])
//			{
//				continue;
//			}
//			auto f = std::bind(*fun);
//			s_Threads[i] = std::shared_ptr<std::thread>(new std::thread(f));
//			s_Tasks.pop_front();
//			break;
//		}
//	}
//}
//
