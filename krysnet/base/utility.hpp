#ifndef __UTILITY__
#define __UTILITY__

#include <type_traits>

template<template<typename ... ARGS> class TEMPLATE_TYPE, typename INSTANCE_TYPE>
struct is_instance_of
{
	template<typename NORMAL_TYPE>
	struct trait : std::false_type { };

	template<typename ... SPECIAL>
	struct trait<TEMPLATE_TYPE<SPECIAL...>> : std::true_type { };

	constexpr static bool value = trait<INSTANCE_TYPE>::value;
};

#endif
