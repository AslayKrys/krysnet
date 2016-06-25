#include "krysnet/base/exception.hpp"
#include <fcntl.h>
#include <sys/time.h>
#include <iostream>
#include <boost/scope_exit.hpp>
#include <gtest/gtest.h>
#include <krysnet/base/log_stream.hpp>
#include "krysnet/base/mutex_guard.hpp"


using namespace std::string_literals;
using std::string;


inline char gen_rand_char ()
{
	return static_cast<char> (rand ());
}

std::string rand_str (int len)
{
	std::string str (len, '\0');
	for (auto& iter : str)
	{
		iter = gen_rand_char ();
	}
	return str;
}

template<int buf_size>
string random_append (krys::log_stream<buf_size>& buf)
{
	int method = rand () % 6 + 1;

	switch (method)
	{
	case 1:
		{
			char ch = rand ();
			buf << ch;
			return {ch};
		}
		break;
	case 2:
		{
			uint8_t ch = rand ();
			auto left = buf.avail ();
			buf << ch;
			return std::to_string (ch).substr (0, left);
		}
		break;
	case 3:
		{
			int16_t sh = rand ();
			auto left = buf.avail ();
			buf << sh;
			return std::to_string (sh).substr (0, left);
		}
		break;
	case 4:
		{
			uint16_t sh = rand ();
			auto left = buf.avail ();
			buf << sh;
			return std::to_string (sh).substr (0, left);
		}
		break;
	case 5:
		{
			int sh = rand ();
			auto left = buf.avail ();
			buf << sh;
			return std::to_string (sh).substr (0, left);
		}
	case 6:
		{
			uint32_t sh = rand ();
			auto left = buf.avail ();
			buf << sh;
			return std::to_string (sh).substr (0, left);
		}
		break;
	case 7:
		{
			int len = rand () % buf.capacity ();

			std::string str (len, '\0');
			for (auto& ch : str) { ch = rand ();}
			auto left = buf.avail ();
			buf << str;
			return str.substr (0, left);
		}

	default:
		return {};
	}

	return {};
}


TEST (test_fixed_buffer, handle_lots_of_input)
{
	krys::log_stream<4096> buf;

	for (int i = 0; i < 10000; i++, buf.reset ())
	{
		auto total_str = ""s;

		while (!buf.full ())
		{
			auto str_tmp = random_append<4096> (buf);
			total_str += str_tmp;

			ASSERT_TRUE (buf.view () == total_str) << "buf view size:" << buf.length ();
			ASSERT_TRUE (buf.view () == total_str);
			ASSERT_TRUE (buf.str () == total_str);
			ASSERT_TRUE (buf.length () == static_cast<long>(total_str.length ()));
			ASSERT_TRUE (buf.avail () == buf.capacity () - total_str.length ());
		}

		ASSERT_TRUE (buf.length () == buf.capacity ());
		ASSERT_TRUE (buf.avail () == 0);

		buf.reset ();
		ASSERT_TRUE (buf.view () == "");
		ASSERT_TRUE (buf.str () == "");
		ASSERT_TRUE (buf.length () == 0);
		ASSERT_TRUE (buf.avail () == buf.capacity ());
	}

}


int main (int argc, char** argv)
{
	testing::InitGoogleTest (&argc, argv);
	return RUN_ALL_TESTS ();
}
