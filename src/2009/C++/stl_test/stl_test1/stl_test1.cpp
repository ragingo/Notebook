// stl_test1.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"


void out(int value){
	std::cout << value << std::endl;
}

class Functions{
public:
	void out(int value){
		std::cout << value << std::endl;
	}
};

struct IsOdd : std::unary_function<int, bool> {
	bool operator()(int val){
		return val%2 == 1;
	}
};

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	vector<string> v;
	v.push_back("aa");
	v.push_back("bbb");
	v.push_back("cccc");
	v.push_back("ddddd");

	vector<int> v2(v.size());

	transform(v.begin(), v.end(), v2.begin(), mem_fun_ref(&string::length));

	for_each(v2.begin(), v2.end(), out);
	//for_each(v2.begin(), v2.end(), ptr_fun(out));

	for_each(
		v2.begin(),
		v2.end(),
		bind1st(mem_fun1(&Functions::out), v2)
	);

	IsOdd obj;
	IsOdd::argument_type input;
	IsOdd::result_type   result;

	cin >> input;
	result = obj(input);
	cout << (result ? "odd" : "even") << endl;

	return 0;
}

