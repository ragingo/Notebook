#pragma once


template<typename T1, typename...Types>
struct param_pack_first
{
	typedef T1 type;
};

template<>
struct param_pack_first<void>
{
};

template<typename T1, typename...Types>
struct param_pack_last
{
	typedef typename param_pack_last<Types...>::type type;
};


template<typename T1>
struct param_pack_last<T1>
{
	typedef T1 type;
};


template<typename...Types>
struct param_pack_type_holder
{
	static const size_t param_pack_count = sizeof...(Types);
	typedef typename param_pack_first<Types...>::type first_type;
	typedef typename param_pack_last<Types...>::type last_type;
};

template <>
struct param_pack_type_holder<>
{
	static const size_t param_pack_count = 0;
	typedef void* first_type;
	typedef void* last_type;
};

