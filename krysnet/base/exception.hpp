#include "krysnet/base/usings.hpp"
#include <execinfo.h>
#include <string.h>
#include <cxxabi.h>
#include <sstream>




namespace krys
<%
class exception : public std::exception
{
public:

	explicit exception (const string& what): message_ (what)
	{
		fill_backtrace ();
	}

	explicit exception (const char* what) :exception (string {what})
	{
	}

	virtual compl exception () throw ()
	{
	}
	virtual const char* what () const throw () override
	{
		return message_.data ();
	}
	const char* stack_info () const noexcept
	{
		return stack_info_.data ();
	}

private:
	void fill_backtrace () noexcept
	{
		std::array<void*, 200> buffers;
		int len = ::backtrace (buffers.data (), buffers.size ());
		std::unique_ptr<char*[]> symbols { ::backtrace_symbols (buffers.data (), len)};

		std::array<char, 31 + 1> module;
		std::array<char, 127 + 1> func;
		std::array<char, 15 + 1> addr;

		for (int i = 0; i < len; i++)
		{
			sscanf (symbols[i], "%31[^(](%127[^+]", module.data (), func.data ());

			int status;
			unique_ptr<char[]> pretty_function {abi::__cxa_demangle (func.data (), 0, 0, &status)};
			snprintf (addr.data (), 15, "%p", buffers[i]);

			if (pretty_function or ::strcmp ("main", func.data ()) == 0)
			{
				stack_info_ += "module:";
				stack_info_ += module.data ();
				stack_info_ += " addr:";
				stack_info_ += addr.data ();
				stack_info_ += "   ";

				if (pretty_function)
				{
					stack_info_ += pretty_function.get ();
				}
				else
				{
					stack_info_ += func.data ();
				}

				stack_info_ += "\n";
			}
		}
	}

private:
	string message_;
	string stack_info_;
};

%>
