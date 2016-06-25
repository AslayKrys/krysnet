#ifndef __FILE_UTILS__
#define __FILE_UTILS__

#include <stdio.h>
#include <memory>
#include <type_traits>
#include <experimental/optional>
#include <experimental/string_view>
#include "krysnet/base/array_view.hpp"

namespace krys 
<%
using std::experimental::basic_string_view;
using std::experimental::string_view;
using std::experimental::optional;
using std::string;



struct fclose_deleter
{
	void operator ()(FILE* fp)
	{
		fclose (fp);
	}
};

using file_ptr = std::unique_ptr<FILE, fclose_deleter>;



inline file_ptr open_file (const char* path, const char* mode)
{
	return file_ptr{fopen (path, mode)};
}

template<typename T>
auto write_to_file (FILE* fp, array_view<T> view)
{
	static_assert (!std::is_pointer<T>::value, "writing an array of pointer to file");
	static_assert (std::is_pod<T>::value, "array type is not POD");
	return fwrite (view.data (), sizeof (T), view.size (), fp);
}

inline optional<string> read_all (const string& filename)
{
	return {};
}

%>


#endif
