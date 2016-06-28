#ifndef __UTILITY__
#define __UTILITY__

#include <type_traits>
#include "krysnet/base/array_view.hpp"

template<template<typename ... ARGS> class TEMPLATE_TYPE, typename INSTANCE_TYPE>
struct is_instance_of
{
	template<typename NORMAL_TYPE>
	struct trait : std::false_type 
	{ 
	};

	template<typename _1ST_ARG, typename ... SPECIAL>
	struct trait<TEMPLATE_TYPE<_1ST_ARG, SPECIAL...>> : std::true_type 
	{ 
	};

	constexpr static bool value = trait<INSTANCE_TYPE>::value;
};


template<typename TARGET, typename _1ST_ARG, typename ... ARGS>
struct type_count
{
	constexpr static unsigned value = 
		(std::is_same<typename std::decay<TARGET>::type, typename std::decay<_1ST_ARG>::type>::value ?  type_count<TARGET, ARGS...>::value + 1: type_count<TARGET, ARGS...>::value);
};

template<typename TARGET, typename _1ST_ARG>
struct type_count<TARGET, _1ST_ARG>
{
	constexpr static unsigned value = 
		(std::is_same<typename std::decay<TARGET>::type, typename std::decay<_1ST_ARG>::type>::value ?  1 : 0);
};

#define CHECK_MANDATORY_PARA(type,arg_pack) \
	do { \
	static_assert(type_count<type,arg_pack...>::value > 0, #type " must be passed");\
	static_assert(type_count<type,arg_pack...>::value < 2, #type " must be passed only once");\
	}\
		while (0)

#define CHECK_OPTIONAL_PARA(type,arg_pack) \
	do { \
	static_assert(type_count<type,arg_pack...>::value < 2, #type " must be passed only once");\
	}\
		while (0)



template<typename ... ARGS>
struct args_helper
{
	template<typename _1ST_ARG, typename ... PACK>
		struct para_pack
		{
			constexpr static bool value = type_count<_1ST_ARG, ARGS...>::value != 1 ? false : para_pack<PACK...>::value;
		};

	template<typename _1ST_ARG>
		struct para_pack<_1ST_ARG>
		{
			constexpr static bool value = type_count<_1ST_ARG, ARGS...>::value == 1;
		};
};


template<typename T, typename U>
struct get_value 
{
	static void _ (T&t, U&&u)
	{
		
	}
};

template<typename T>
struct get_value <T, T>
{
	static void _ (T&t, T&&u)
	{
		t = std::move (u);
	}
};

template<typename T, typename U, typename ... ARGS>
inline void get_paras (T& t, U&& u, ARGS&& ... args)
{
	get_value<T, U>::_ (t, std::forward<U>(u));
	get_paras<T, ARGS...> (t, std::forward<ARGS>(args)...);
}

template<typename T, typename U>
inline void get_paras (T& t, U&& u)
{
	get_value<T, U>::_ (t, std::forward<U>(u));
}


//struct header
//{
//	header () = default;
//	header (std::initializer_list<std::pair<std::string, std::string>>&&list)
//	{
//		for (auto&& iter : list)
//		{
//			value_.insert (iter);
//		}
//	}
//	header& operator = (header&& other)
//	{
//		value_ = std::move (other.value_);
//		return *this;
//	}
//	header (header&& other)
//	{
//		value_ = std::move (other.value_);
//	}
//	std::map<std::string, std::string> value_;
//};




//template<typename ... ARGS>
//inline void func (ARGS&& ... args)
//{
//	CHECK_MANDATORY_PARA (buffer, ARGS);
//	CHECK_MANDATORY_PARA (socket, ARGS);
//	CHECK_MANDATORY_PARA (header, ARGS);
//	static_assert (args_helper<buffer, socket, header>::template para_pack<ARGS...>::value, "extra parameter is not acceptable" );
//
//	buffer buff;
//	get_paras (buff, std::forward<ARGS>(args)...);
//	socket sock;
//	get_paras (sock, std::forward<ARGS>(args)...);
//	header header_content;
//	get_paras (header_content, std::forward<ARGS>(args)...);
//
//	std::cout << buff.value_.data () << std::endl;
//	std::cout << sock.value_ << std::endl;
//
//	for (auto&& iter : header_content.value_)
//	{
//		std::cout << iter.first << " " << iter.second << std::endl;
//	}
//}

#endif
