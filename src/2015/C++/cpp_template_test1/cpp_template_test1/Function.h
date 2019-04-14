#pragma once
#include <array>
#include <initializer_list>
#include <functional>
#include <type_traits>
#include <stdarg.h>
#include "template_util.h"


class Delegate
{
public:
	enum TypeID
	{
		TypeID_Delegate,
		TypeID_Function,
		TypeID_Action,
	};

	Delegate()
	{
	}

	virtual void* Invoke(void*, ...) = 0;

	virtual TypeID GetID() const
	{
		return TypeID_Delegate;
	}

	virtual bool HasReturnValue() = 0;

	virtual bool HasArguments() = 0;

	virtual size_t GetArgumentCount() = 0;
};


template<typename R = void, typename...Types>
class Function : public Delegate
{
public:
	typedef R return_type;
	using param_pack_info = param_pack_type_holder <Types...>;
	typedef typename param_pack_info::first_type first_type;
	typedef typename param_pack_info::last_type last_type;

#define FUNC_SIGNATURE return_type(Types...)

	Function()
	{
	}

	Function(std::function<FUNC_SIGNATURE> func)
	{
		m_Func = func;
	}

	Function(const Function<return_type, Types...>& func)
	{
		m_Func = func.m_Func;
	}

	virtual ~Function()
	{
	}

	return_type operator()(Types...args)
	{
		if (!m_Func) {
			return (return_type)NULL;
		}
		return m_Func(args...);
	}

	virtual void* Invoke(void* arg, ...) override
	{
		if (param_pack_info::param_pack_count == 0)
		{
			return nullptr;
		}

		if (!arg) {
			return nullptr;
		}

		static const size_t size = param_pack_info::param_pack_count > 0 ? param_pack_info::param_pack_count : 1;

		va_list list;
		va_start(list, size);

		std::array<first_type, size> arr;
		for (size_t i=0; i<size; i++)
		{
			arr[i] = (first_type)va_arg(list, first_type);
		}

		va_end(list);

		return nullptr;
	}

	bool HasReturnValue() override
	{
		return !std::is_void<return_type>();
	}

	bool HasArguments() override
	{
		return param_pack_info::param_pack_count > 0;
	}

	size_t GetArgumentCount() override
	{
		return param_pack_info::param_pack_count;
	}

	virtual Delegate::TypeID GetID() const override
	{
		return Delegate::TypeID_Function;
	}

protected:
	std::function<FUNC_SIGNATURE> m_Func;

#undef FUNC_SIGNATURE
};


template<typename...Types>
class Action : public Function<void, Types...>
{
public:
#define FUNC_SIGNATURE void(Types...)
	Action()
	{
	}

	//Action(const Function<FUNC_SIGNATURE>& func) : Function<R, Types...>(func)
	//{
	//}

	virtual ~Action()
	{
	}

	virtual Delegate::TypeID GetID() const override
	{
		return Delegate::TypeID_Action;
	}

#undef FUNC_SIGNATURE
};

