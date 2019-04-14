
#include "stdafx.h"


template<typename Container>
class IEnumerator : 
	public std::iterator<
		typename Container::iterator::iterator_category,
		typename Container::iterator::value_type,
		typename Container::iterator::difference_type,
		typename Container::iterator::pointer,
		typename Container::iterator::reference>
{
public:
	typedef typename Container::iterator iterator;
	virtual iterator begin() = 0;
	virtual iterator end() = 0;
};

template<typename T>
class IEnumerable : public IEnumerator<std::vector<T>>
{
private:
	std::vector<T> _seq;
public:
	IEnumerable(const std::vector<T>& seq) : _seq(seq)
	{
	}
	IEnumerable(const IEnumerable<T>& obj)
	{
		seq = obj._seq;
	}
	virtual iterator begin()
	{
		return _seq.begin();
	}
	virtual iterator end()
	{
		return _seq.end();
	}
	int Count() const
	{
		return _seq.size();
	}
};



//template<typename T>
//class Enumerable
//{
//private:
//	typedef bool (*Predicate)(T);
//	std::vector<T> _sequence;
//public:
//	Enumerable(const std::vector<T>& sequence)
//	{
//		std::copy(sequence.begin(), sequence.end(), std::back_inserter(_sequence));
//	}
//	const std::vector<T>& Get() const
//	{
//		return _sequence;
//	}
//	bool All(const Predicate& pred) const
//	{
//		int ret = std::count_if(_sequence.begin(), _sequence.end(), pred);
//		return ret == _sequence.size();
//	}
//	bool Any(const Predicate& pred) const
//	{
//		int ret = std::count_if(_sequence.begin(), _sequence.end(), pred);
//		return ret > 0;
//	}
//	bool Contains(const Predicate& pred)
//	{
//		return this->Any(pred);
//	}
//	Enumerable* Where(const Predicate& pred)
//	{
//		_sequence.erase(std::remove_if(_sequence.begin(), _sequence.end(), pred), _sequence.end());
//		return this;
//	}
//};