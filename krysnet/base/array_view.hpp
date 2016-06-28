#ifndef __ARRAY_VIEW__
#define __ARRAY_VIEW__
#include <type_traits>
#include <iterator>
#include <vector>
#include <cstddef>
#include "krysnet/base/exception.hpp"
#include <array>
#include <string>
#include <stddef.h>

namespace krys
<%
using namespace std::string_literals;

struct oob_action
{
	void operator () (const char* func, size_t access, size_t len)
	{
		std::string str = 
			"buffer overflow at "s + func + "attempt to access index:" + 
			std::to_string (access) + " but only with size:" + std::to_string (len);

		throw ::krys::exception {str.data ()};
	}
};

template<typename DATA, typename OUT_OF_BOUND_EXEC, typename SIZE_TYPE>
class __array_view__
{
public:
	using value_type				=				DATA;
	using pointer					=				value_type*;
	using const_pointer				=				const pointer;
	using iterator					=				DATA*;
	using const_iterator			=				const iterator;
	using reference					=		  		value_type&;
	using const_reference			=				value_type&;
	using reverse_iterator			=				std::reverse_iterator<iterator>;
	using const_reverse_iterator	=				std::reverse_iterator<const_iterator>;
	using size_type					=				size_t;
	using difference_type			=				ptrdiff_t;

	__array_view__ () = delete;

	template<typename ... ARGS>
	__array_view__ (const std::vector<DATA, ARGS...>& vec) noexcept
	: data_ (vec.data ()), size_ (vec.size ()) {}

	template<typename ... ARGS>
	__array_view__ (const std::basic_string<DATA, ARGS...>& vec) noexcept
	: data_ (vec.data ()), size_ (vec.size ()) {}

	template<std::size_t size>
	__array_view__ (DATA(&data) [size]) noexcept
	: data_ (data), size_ (size) {}

	
	template<std::size_t size>
	__array_view__ (std::array<DATA, size>& array) noexcept
	:data_ (array.data ()), size_ (array.size ()) {}

	template<std::size_t size>
	__array_view__ (const std::array<DATA, size>& array) noexcept
	:data_ (array.data ()), size_ (array.size ()) {}

	__array_view__ (const __array_view__&) = default;

	constexpr iterator begin () { return data_;}
	constexpr iterator end () { return data_ + size_;}
	constexpr const_iterator cbegin () const { return data_;}
	constexpr const_iterator cend () const { return data_ + size_;}
	constexpr reverse_iterator rbegin () { return std::reverse_iterator<iterator> (end ());}
	constexpr reverse_iterator rend () { return std::reverse_iterator<iterator> (begin ());}
	constexpr size_type size () const{ return size_; }
	constexpr size_type length () const { return size_; }
	constexpr bool empty () const{ return size_ == 0; }
	constexpr DATA& operator [] (size_type size) { return at (size); }
	constexpr const DATA& operator [] (size_type size) const { return at (size); }

	constexpr DATA& front () { return *data_;}
	constexpr const DATA& front ()const { return *data_;}

	constexpr DATA& back () { return *(data_ + size_ - 1);}
	constexpr const DATA& back ()const { return *(data_ + size_ - 1);}

	constexpr DATA* data () { return data_;}
	constexpr const DATA* data ()const { return data_;}

	constexpr const DATA& at (size_type size) const
	{
		if (size >= size_)
		{
			OUT_OF_BOUND_EXEC {} ("__array_view__::at (size_type size)", size, size_);
		}
		return data_[size];
	}

	constexpr DATA& at (size_type size)
	{
		return const_cast<DATA&>(const_cast<const __array_view__*> (this)->at (size));
	}


private:
	DATA* data_;
	size_type size_;
};
template<typename DATA_TYPE>
using array_view = __array_view__<DATA_TYPE, oob_action, uint32_t>;

%>

#endif
