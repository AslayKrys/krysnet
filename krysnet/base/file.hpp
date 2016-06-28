#ifndef __FILE_UTILS__
#define __FILE_UTILS__

#include <stdio.h>
#include <memory>
#include "krysnet/base/utility.hpp"
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

struct buffer 
{
	template<typename ... ARGS>
	buffer (ARGS&& ... args) : view_ (std::forward<ARGS>(args)...) {}
	string_view view_;
};


using FILE_HANDLE = FILE*;

template<typename ... ARGS>
auto write_to_file (ARGS&& ... args)
{
	CHECK_MANDATORY_PARA (buffer, ARGS);
	CHECK_MANDATORY_PARA (FILE*, ARGS);
	buffer buf;
	FILE* fp;
	get_paras (buf, std::forward<ARGS>(args)...);
	get_paras (fp, std::forward<ARGS> (args)...);

	
	return fwrite (buf.view_.data (), sizeof (char), buf.view_.size (), fp);
}

inline optional<string> read_all (string_view filename)
{
	string buffer_read;
	auto fp = open_file (filename.data (), "rb");
	
	if (fp == nullptr)
	{
		return {};
	}
	
	if (::fseek (fp.get (), 0, SEEK_END) != 0)
	{
		return {};
	}
	
	signed length;
	if ((length = ::ftell (fp.get ())) == -1)
	{
		return {};
	}
	
	if (::fseek (fp.get (), 0, SEEK_SET) != 0)
	{
		return {};
	}
	buffer_read.resize (length);

	auto read_len = ::fread (const_cast<char*> (buffer_read.data ()), 
			1, buffer_read.length (), fp.get ());

	if (read_len != buffer_read.length ())
	{
		return {};
	}
	
	return std::move (buffer_read);
}

%>


#endif
