#include <stdint.h>
#include <utility>
#include <sys/time.h>
#include <inttypes.h>
#include <string>
#include <boost/operators.hpp>
#include <time.h>


namespace krys
<%
using std::string;

class timestamp : public boost::less_than_comparable<timestamp>
{
public:
	constexpr static int microseconds_per_second = 1000 * 1000;
public:
	timestamp () noexcept: microseconds_since_epoc_ (0)
	{

	}
	explicit timestamp (uint64_t microseconds_since_epoc)  noexcept: microseconds_since_epoc_ (microseconds_since_epoc) 
	{ 

	}

	void swap (timestamp& that) noexcept
	{
		std::swap (microseconds_since_epoc_, that.microseconds_since_epoc_);
	}

	string to_string () noexcept
	{
		string str (31, '\0');

		uint64_t sec = microseconds_since_epoc_ / microseconds_per_second;
		uint64_t microseconds = microseconds_since_epoc_ % microseconds_per_second;

		int len = snprintf (const_cast<char*>(str.data ()), 32, "%" PRId64 ".%06" PRId64 "", sec, microseconds);
		str.resize (len);

		return str;
	}

	string to_formatted_string (bool show_microseconds = true) noexcept
	{
		string str (31, '\0');
		time_t seconds = static_cast<time_t> (microseconds_since_epoc_ / microseconds_per_second);
		struct tm tm_time;
		gmtime_r (&seconds, &tm_time);

		int len;
		if (show_microseconds)
		{
			int microseconds = static_cast<int> (microseconds_since_epoc_ % microseconds_per_second);

			len = snprintf (const_cast<char*> (str.data ()), 32, "%04d%02d%02d %02d:%02d:%02d.%06d",
					tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday,
					tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec, microseconds);
		}
		else
		{
			len = snprintf (const_cast<char*> (str.data ()), 32, "%04d%02d%02d %02d:%02d:%02d",
					tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday,
					tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec);
		}

		str.resize (len);
		return str;
	}

	bool valid () noexcept
	{
		return microseconds_since_epoc_ > 0;
	}

	uint64_t microseconds_since_epoc () noexcept
	{
		return microseconds_since_epoc_;
	}

	time_t seconds_since_epoc () noexcept
	{
		return static_cast<time_t> (microseconds_since_epoc_) / microseconds_per_second;
	}

public: //static

	static timestamp from_unix_time (time_t t, int offset_microseconds = 0) noexcept
	{
		return timestamp {static_cast<uint64_t> (t) * microseconds_per_second + offset_microseconds};
	}

	static timestamp now () noexcept
	{
		struct timeval tv;
		gettimeofday (&tv, nullptr);
		uint64_t seconds = tv.tv_sec;
		return timestamp (seconds * microseconds_per_second + tv.tv_usec);
	}

private:
	uint64_t microseconds_since_epoc_;
};

inline bool operator < (timestamp l, timestamp r) noexcept
{
	return l.microseconds_since_epoc () < r.microseconds_since_epoc ();
}

inline bool operator != (timestamp l, timestamp r) noexcept
{
	return l.microseconds_since_epoc () != r.microseconds_since_epoc ();
}

inline double time_diff (timestamp high, timestamp low) noexcept
{
	auto diff = high.microseconds_since_epoc () - low.microseconds_since_epoc ();

	return static_cast<double> (diff) / timestamp::microseconds_per_second;
}

%>
