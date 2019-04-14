
#include "stdafx.h"


template<class T>
class enumerable
{
private:
	typedef std::tr1::function<bool (T)> Predicate;
	std::vector<T> *_list;
public:
	enumerable(std::vector<T>& list)
	{
		_list = &list;
	}
	//bool all(bool (*pred)(T arg)) const
	//{
	//	return this->all(Predicate(pred));
	//}
	//bool all(const Predicate& pred) const
	//{
	//	for(std::vector<T>::const_iterator iter = _list->begin(); iter != _list->end(); iter++)
	//	{
	//		if(!pred(*iter))
	//		{
	//			return false;
	//		}
	//	}
	//	return true;
	//}
	//bool any(bool (*pred)(T arg)) const
	//{
	//	return this->any(Predicate(pred));
	//}
	//bool any(const Predicate& pred) const
	//{
	//	bool ret = false;
	//	for(std::vector<T>::const_iterator iter = _list->begin(); iter != _list->end(); iter++)
	//	{
	//		if(pred(*iter))
	//		{
	//			ret = true;
	//		}
	//	}
	//	return ret;
	//}
	T average() const
	{
		T val = this->sum() / _list->size();
		return val;
	}
	//bool contains(T value) const
	//{
	//	return this->any([&](T v)->bool{ return v == value; });
	//}
	int count() const
	{
		return _list->size();
	}
	int count(const Predicate& pred)
	{
		return this->where(pred)->count();
	}
	enumerable* concat(const std::vector<T>& v)
	{
		std::copy(v.begin(), v.end(), std::back_inserter(*_list));
		return this;
	}
	const std::vector<T>* get() const
	{
		return _list;
	}
	T max() const
	{
		T val = *std::max_element(_list->begin(), _list->end());
		return val;
	}
	T min() const
	{
		T val = *std::min_element(_list->begin(), _list->end());
		return val;
	}
	enumerable* orderby()
	{
		std::sort(*_list.begin(), *_list.end());
		return this;
	}
	enumerable* orderbyDescending()
	{
		std::sort(_list->begin(), _list->end(), std::greater<T>());
		return this;
	}
	enumerable* reverse()
	{
		std::reverse(_list->begin(), _list->end());
		return this;
	}
	enumerable* skip(const int& count)
	{
		std::vector<T> v;
		std::copy(_list->begin() + count, _list->end(), std::back_inserter(v));
		_list->clear();
		_list->assign(v.begin(), v.end());
		return this;
	}
	T sum() const
	{
		T val = std::accumulate(_list->begin(), _list->end(), (T)0, std::plus<T>());
		return val;
	}
	enumerable* where(bool (*pred)(T arg))
	{
		return this->where(Predicate(pred));
	}
	enumerable* where(const Predicate& pred)
	{
		std::vector<T> result;
		for(std::vector<T>::const_iterator iter = _list->begin(); iter != _list->end(); iter++)
		{
			if(pred(*iter))
			{
				result.push_back(*iter);
			}
		}
		_list->clear();
		_list->assign(result.begin(), result.end());
		return this;
	}
	std::string toString()
	{
		std::stringstream ss;
		ss.str("");
		ss << '[';
		for(std::vector<T>::const_iterator iter = _list->begin(); iter != _list->end(); iter++)
		{
			ss << *iter;
			if(iter+1 != _list->end())
			{
				ss << ',';
			}
		}
		ss << ']';
		std::string str;
		ss >> str;
		return str;
	}
};

int _tmain1(int argc, _TCHAR* argv[])
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	std::vector<int> v1(10);
	std::generate(v1.begin(), v1.end(), [&]()->int{ static int i=0;return ++i; });
	std::vector<int> v2(20);
	std::generate(v2.begin(), v2.end(), [&]()->int{ static int i=0;return ++i; });
	std::vector<double> v3(10);
	std::generate(v3.begin(), v3.end(), [&]()->double{ static double i=0.0;return ++i; });

	//std::cout << enumerable<int>(v1).contains(3) << std::endl;

	//std::cout << 
	//	enumerable<int>(v1)
	//	.reverse()
	//	->skip(2)
	//	->toString() << 
	//std::endl;

	//std::cout << enumerable<int>(v1).sum() << std::endl;
	//std::cout << enumerable<int>(v1).average() << std::endl;
	//std::cout << enumerable<int>(v1).max() << std::endl;
	//std::cout << enumerable<int>(v1).min() << std::endl;

	//std::cout << enumerable<double>(v3).sum() << std::endl;
	//std::cout << enumerable<double>(v3).average() << std::endl;
	//std::cout << enumerable<double>(v3).max() << std::endl;
	//std::cout << enumerable<double>(v3).min() << std::endl;

	//std::cout << 
	//	enumerable<int>(v1)
	//		.where(iseven)
	//		->orderbyDescending()
	//		->get()
	//		->size() << 
	//std::endl;

	//std::cout << 
	//	enumerable<int>(v1)
	//		.where(iseven)
	//		->orderbyDescending()
	//		->concat(v2)
	//		->where(isodd)
	//		->toString() << 
	//std::endl;

	return 0;
}