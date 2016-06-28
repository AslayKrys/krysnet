#include <iostream>
#include "krysnet/base/array_view.hpp"
#include "krysnet/base/file.hpp"

using krys::FILE_HANDLE;
using krys::buffer;

int main (int argc, char** argv)
{
	std::string str = "12345";

	auto op_str = krys::read_all ("123.txt");
	if (!op_str)
	{
		std::cout << "failed reading file" << std::endl;
		return 1;
	}


	std::cout << *op_str << std::endl;
	
	return 0;
}
