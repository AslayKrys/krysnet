#include <iostream>
#include "krysnet/base/array_view.hpp"
#include "krysnet/base/file.hpp"

using krys::FILE_HANDLE;
using krys::buffer;

int main (int argc, char** argv)
{
	std::string str = "12345";

	auto fp = krys::open_file ("123.txt", "a+");
	if (fp == nullptr)
	{
		return 0;
	}

	krys::write_to_file (
			buffer {str}, 
			FILE_HANDLE {fp.get ()}
			);
	
	
	return 0;
}
