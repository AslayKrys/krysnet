#include <array>
#include <experimental/string_view>
#include <assert.h>
#include <algorithm>


namespace krys
<%

using std::experimental::string_view;

template<size_t buffer_size>
class log_stream
{
	static const char digits[];
	static constexpr const char* zero = digits + 9;
	
public:
	log_stream () = default;
	log_stream (const log_stream&) = delete;
	compl log_stream () = default;

	auto avail  () const noexcept
	{
		int diff = arr_buffer_.end () - cur_;
		assert (diff >= 0);
		return static_cast<unsigned> (diff);
	}

	bool full ()
	{
		return avail () == 0;
	}

	void reset () noexcept
	{
		cur_ = arr_buffer_.begin ();
	}

	constexpr auto capacity () const noexcept
	{
		return buffer_size;
	}

	auto length () const noexcept
	{
		return cur_ - arr_buffer_.begin ();
	}

	log_stream& operator << (char ch)
	{
		if (!full ())
		{
			*cur_ = ch;
			++ cur_;
		}
		return *this;
	}

	template<typename T>
	typename std::enable_if<std::is_integral<T>::value, log_stream>::type& operator << (T value)
	{
		static_assert (sizeof (typename std::enable_if<!std::is_same<T, char>::value, char>::type) == 1, "");
		if (avail () == 0)
		{
			return *this;
		}

		std::array<char, sizeof value * 3> tmp_buf;
		auto iter = tmp_buf.begin ();
		auto i = value;

		do
		{
			int lsd = static_cast<int> (i % 10);
			i /= 10;
			*iter = zero[lsd];
			++ iter;
		}while (i != 0);

		if (value < 0)
		{
			*iter = '-';
			iter ++;
		}

		auto diff = iter - tmp_buf.begin ();
		std::reverse (tmp_buf.begin (), iter);
		if (avail () < diff)
		{
			diff = avail ();
		}
		append (tmp_buf.data (), static_cast<unsigned long>(diff));

		return *this;
	}

	template<typename ... ARGS>
	log_stream& operator << (ARGS&& ... args)
	{
		append (std::forward<ARGS>(args)...);
		return *this;
	}

	template<typename ... ARGS>
    auto append (ARGS&& ... args) noexcept
	{
		string_view view {std::forward<ARGS>(args)...};
		decltype (view.begin ()) end;
		auto left = avail ();

		if (view.length () <= left)
		{
			end = view.end ();
		}
		else
		{
			end = view.begin () + left;
		}

		cur_ = std::copy (view.begin (), end, cur_);
	}

	std::string str () const noexcept
	{
		return view ().to_string ();
	}

	auto view () const noexcept
	{
		return string_view (arr_buffer_.data(), length ());
	}

private:
	std::array<char, buffer_size> arr_buffer_;
	typename decltype (arr_buffer_)::iterator cur_ = arr_buffer_.begin ();
};


template<size_t buffer_size>
constexpr const char log_stream<buffer_size>::digits[] = "9876543210123456789";

%>
