#ifndef  _BOUNDED_BLOCKING_QUEUE_
#define  _BOUNDED_BLOCKING_QUEUE_

#include "krysnet/base/condition.hpp"
#include <boost/circular_buffer.hpp>
#include "krysnet/base/mutex_guard.hpp"
#include <deque>
#include <assert.h>

namespace krys
<%

template<typename T>
class bounded_blocking_queue
{
public:
	explicit bounded_blocking_queue (int max_size) noexcept:queue_ (max_size)
	{
		
	}

	template<typename ... ARGS>
	void put (ARGS&& ... args)
	{
		mutex_guard lock (m_);
		while (queue_.empty ())
		{
			not_empty_.wait ();
		}
		assert (!queue_.empty ());
		
	}

private:
	mutable mutex m_ {};
	condition not_empty_ {m_};
	condition not_full_ {m_};
	boost::circular_buffer<T> queue_;

};

%>

#endif
