
#include <functional>
#include <iostream>
#include <memory>
#include <vector>
#include <thread>

#include "Function.h"

class TestClass
{
public:
	void print(const char* str){ std::cout << str << std::endl; }
	int add(int a, int b){ return a+b; }
	int add(int a, int b, int c){ return a+b+c; }
	int add(int a, int b, int c, int d){ return a+b+c+d; }
};

template<typename T>
inline T add(T a, T b)
{
	return a + b;
}

using namespace std::placeholders;


int main()
{

	TestClass instance;

	Function<> f1;
	//Function<> f2 = []{ std::cout << "f2" << std::endl; };
	//Function<void, int> f3 = std::function<void(int)>([](int a){ std::cout << "f3:" << a << std::endl; });
	//Function<void, const char*> f4 = std::bind(&TestClass::print, &instance, _1);
	//Function<int, int> f5 = [](int a)->int{ return a; };
	//Function<int, int, int> f6 = std::bind(static_cast<int(TestClass::*)(int, int)>(&TestClass::add), &instance, _1, _2);
	//Function<int, int, int, int> f7 = std::bind(static_cast<int(TestClass::*)(int, int)>(&TestClass::add), &instance, _1, _2);
	//Function<int, int, int, int> f7_2 = std::bind(static_cast<int(TestClass::*)(int, int, int)>(&TestClass::add), &instance, _1, _2, _3);
	//Function<int, int, int> f8 = &add<int>;
	//Function<double, double, double> f9 = &add<double>;
	//Function<> f10 = f2;
	//Function<int, int, int> f11 = std::bind(f5, _1);
	//Function<int, int> f12 = std::bind(f11, _1, _1);
	//Function<int, int, Function<int, int>> f13 = [](int a, Function<int, int> b){ return a + b(a) + 1; };
	//auto f14 = Function<int, int, int>(&add<int>);
	//auto f15 = Function<int, int, int>([](int a, int b){ return a + b; });
	//auto f16 = f15;
	//auto f17 = Function<int>([](){ return 17; });
	//auto f18 = Function<int, int, int>(std::bind(f16, _1, _2));
	//auto f19 = Function<int, int, int>(f18);
	//Action<> f20;
	//Action<> f21 = f2;
	//Action<int> f22 = std::function<void(int)>([](int a){ std::cout << "f22:" << a << std::endl; });
	//Action<int> f23 = Function<void, int>(f22);
	//Action<int> f24 = Function<void, int>([](int a){ std::cout << "f24:" << a << std::endl; });
	//std::vector<std::shared_ptr<Delegate>> f25 = {
	//	//std::shared_ptr<Delegate>(new Function<int>(f17)),
	//	//std::shared_ptr<Delegate>(new Function<int, int>(f5)),
	//	//std::shared_ptr<Delegate>(new Function<int, int, int>(f6)),
	//	//std::shared_ptr<Delegate>(new Function<int, int, int, int>(f7)),
	//	//std::shared_ptr<Delegate>(new Function<>(f1)),
	//	//std::shared_ptr<Delegate>(new Function<>(f2)),
	//	//std::shared_ptr<Delegate>(new Action<>(f20)),
	//	//std::shared_ptr<Delegate>(new Action<>(f21)),
	//	//std::shared_ptr<Delegate>(new Action<int>(f22)),
	//};

	//f1();
	//f2();
	//f3(3);
	//f4("f4");
	//std::cout << "f5:" << f5(5) << std::endl;
	//std::cout << "f6:" << f6(2, 4) << std::endl;
	//std::cout << "f7:" << f7(1, 2, 4) << std::endl;
	//std::cout << "f7:" << f7_2(1, 2, 4) << std::endl;
	//std::cout << "f8:" << f8(4, 4) << std::endl;
	//std::cout << "f9:" << f9(4, 5) << std::endl;
	//f10();
	//std::cout << "f11:" << f11(5, 6) << std::endl;
	//std::cout << "f12:" << f12(12) << std::endl;
	//std::cout << "f13:" << f13(6, f12) << std::endl;
	//std::cout << "f14:" << f14(7, 7) << std::endl;
	//std::cout << "f15:" << f15(7, 8) << std::endl;
	//std::cout << "f16:" << f16(8, 8) << std::endl;
	//std::cout << "f17:" << f17() << std::endl;
	//std::cout << "f18:" << f18(9, 9) << std::endl;
	//std::cout << "f19:" << f19(9, 10) << std::endl;
	//f20();
	//f21();
	//f22(22);
	//f23(23);
	//f24(24);
	//for (auto f : f25) {

	//	const char* has_return_str = (f->HasReturnValue() ? "true" : "false");
	//	const char* has_args_str = (f->HasArguments() ? "true" : "false");
	//	const size_t arg_count = f->GetArgumentCount();
	//	std::cout << "id:" << f->GetID() << ", has return:" << has_return_str << ", has args:" << has_args_str << ", arg count:" << arg_count << std::endl;

	//	if (f->HasReturnValue()) {
	//		continue;
	//	}
	//	if (f->HasArguments()) {
	//		continue;
	//	}
	//	switch (f->GetID())
	//	{
	//	case Delegate::TypeID_Action:
	//		{
	//			auto fun_ptr = reinterpret_cast<Action<>*>(f.get());
	//			(*fun_ptr)();
	//		}
	//		break;
	//	default:
	//		break;
	//	}
	//}

	return 0;
}

