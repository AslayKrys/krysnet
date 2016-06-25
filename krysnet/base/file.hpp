#ifndef __FILE_UTILS__
#define __FILE_UTILS__

#include <stdio.h>
#include <memory>
#include <type_traits>
#include <experimental/optional>
#include <experimental/string_view>

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

auto write_to_file (FILE* fp, string_view view)
{
	return fwrite (view.data (), sizeof (char), view.size (), fp);
}

inline optional<string> read_all (const string& filename)
{
	return {};
}

%>


#endif
